void make_beacon(String BSSID, String SSID);
int advert = 0;
int summary = 0;

void madWifi_beacon()
{
    if(advert + 100 > millis())
    {
        return;    
    }

    advert = millis();

    
    String SSID;
    String BSSID;

    if(summary + 1000*10 < millis())
    {
        Serial.println("Advertising Beacons: " + Beacons);  
        summary = millis();
    }

    for(int i=0;i<24;i++)
    {
        SSID = StringIndex(Beacons, " ", i);
        if(SSID.length() > 0)
        {
            make_beacon(BSSID,SSID);
        }
        else
        {
            return;
        }
    }
}

void make_beacon(String BSSID, String SSID)
{
    uint8_t packet[1024];
  
    int _channel = random(1,14); 
    wifi_set_channel(_channel);

    char packet_craft[1024];
    int index = 0;
    int malindex = 0;
    byte buff;

    byte len = SSID.length();



    // Randomize SRC MAC
    packet[index++] = 0x80;
    packet[index++] = 0x00;
    packet[index++] = 0x00;
    packet[index++] = 0x00;

    /** DST_MAC **/
    packet[index++] = 0xff; 
    packet[index++] = 0xff;
    packet[index++] = 0xff;
    packet[index++] = 0xff;
    packet[index++] = 0xff;
    packet[index++] = 0xff;
    /** DST_MAC **/
    
    /** SOURCE_MAC **/
    if(BSSID == "random" || BSSID == "")
    {
        packet[10] = packet[16] = random(256);
        packet[11] = packet[17] = random(256);
        packet[12] = packet[18] = random(256);
        packet[13] = packet[19] = random(256);
        packet[14] = packet[20] = random(256);
        packet[15] = packet[21] = random(256);
        index += 12;    
    }
    else
    {
        BSSID.replace(":","");        
        packet[10] = packet[16] = hexToByte((char*)BSSID.substring(0,2).c_str());
        packet[11] = packet[17] = hexToByte((char*)BSSID.substring(2,4).c_str());
        packet[12] = packet[18] = hexToByte((char*)BSSID.substring(4,6).c_str());
        packet[13] = packet[19] = hexToByte((char*)BSSID.substring(6,8).c_str());
        packet[14] = packet[20] = hexToByte((char*)BSSID.substring(8,10).c_str());
        packet[15] = packet[21] = hexToByte((char*)BSSID.substring(10,12).c_str());
        index += 12;            
    }
    /** SOURCE_MAC **/

    packet[index++] = 0xc0;
    packet[index++] = 0x6c;

    packet[index++] = 0x83;
    packet[index++] = 0x51;
    packet[index++] = 0xf7;
    packet[index++] = 0x8f;
    packet[index++] = 0x0f;
    packet[index++] = 0x00;
    packet[index++] = 0x00;
    packet[index++] = 0x00;

    packet[index++] = 0x64;
    packet[index++] = 0x00;
    packet[index++] = 0x01;
    packet[index++] = 0x04;
    packet[index++] = 0x00;
    packet[index++] = len; //SIZE

    for(int i=0;i<len;i++)
    {
        packet[index++] = SSID.charAt(i);
    }
    
    packet[index++] = 0x01;
    malindex = index;
    packet[index++] = 0x08; //random(0,16);
    packet[index++] = 0x82;
    packet[index++] = 0x84;
    packet[index++] = 0x96;
    packet[index++] = 0x24;
    packet[index++] = 0x30;
    packet[index++] = 0x48;
    packet[index++] = 0x6c;
    packet[index++] = 0x9b;
    packet[index++] = 0x03;
    packet[index++] = 0x01;
    packet[index++] = channel;

    packet[index++] = 0x05;
    packet[index++] = 0x04;
    packet[index++] = 0x00;
    packet[index++] = 0x02;
    packet[index++] = 0x00;
    packet[index++] = 0x00; //0x00
    packet[index++] = 0x07;
    packet[index++] = 0x06;
    packet[index++] = 0x42;
    packet[index++] = 0x4f;
    packet[index++] = 0x20;

    packet[index++] = 0x01;
    packet[index++] = 0x0b;
    packet[index++] = 0x1b;
    packet[index++] = 0x2a;
    packet[index++] = 0x01;
    packet[index++] = 0x04;
    packet[index++] = 0x32;
    packet[index++] = 0x04;
    packet[index++] = 0x30;
    packet[index++] = 0x48;

    packet[index++] = 0x60;
    packet[index++] = 0x6c;
    packet[index++] = 0x30;
    packet[index++] = 0x14;
    packet[index++] = 0x01;
    packet[index++] = 0x00;
    packet[index++] = 0x00;
    packet[index++] = 0x0f;
    packet[index++] = 0xac;
    packet[index++] = 0x04;
    packet[index++] = 0x01;
    packet[index++] = 0x00;

    packet[index++] = 0x00;
    packet[index++] = 0x0f;
    packet[index++] = 0xac;
    packet[index++] = 0x04;
    packet[index++] = 0x01;
    packet[index++] = 0x00;

    packet[index++] = 0x00;
    packet[index++] = 0x0f;
    packet[index++] = 0xac;
    packet[index++] = 0x02;
    packet[index++] = 0x0c;
    packet[index++] = 0x00;

/*
    for(int i=0;i<128;i++)
    {
        packet[index++] = random(255);
    }
*/

    for(int i=0; i<=1;i++)
    {
//        packet[malindex] = i;
        wifi_send_pkt_freedom(packet, index, 0);
        wdt_reset();
        delay(0);
    }
}
