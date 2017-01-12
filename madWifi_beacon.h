void make_beacon(String BSSID, String SSID, int _channel);
int advert = 0;
int summary = 0;

void madWifi_beacon()
{

    if(!trigger(50,&advert))
    {
        return;
    }
  
    led1_blink(50);
    
    String SSID;
    String BSSID;

    if(trigger(1000,&summary) && Beacons.length() > 1)
    {
        Serial.println("Advertising Beacons: " + Beacons);  
    }

    for(int i=0;i<24;i++)
    {
        SSID = StringIndex(Beacons, " ", i);
        if(SSID.length() > 1)
        {
            make_beacon(BSSID,SSID,random(1,14));
        }
        else
        {
            return;
        }
    }
}

void make_beacon(String BSSID, String SSID, int _channel)
{
    uint8_t packet[1024];
    int index = 0;
    byte buff;

    SSID.replace("[RND]",String(random(255)));
    byte len = SSID.length();


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
    packet[index++] = _channel;

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
       wifi_set_channel(_channel);
      for(int i=0;i<3;i++)
      {
          wifi_send_pkt_freedom(packet, index, 0);
      }
}

