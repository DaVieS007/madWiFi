void madWifi_random()
{
    uint8_t packet[1024];
  
    int _channel = random(1,14); 
    wifi_set_channel(_channel);

    char packet_craft[512];
    int index = 0;
    int malindex = 0;
    byte buff;

    packet[index++] = random(255);
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
    packet[10] = packet[16] = random(256);
    packet[11] = packet[17] = random(256);
    packet[12] = packet[18] = random(256);
    packet[13] = packet[19] = random(256);
    packet[14] = packet[20] = random(256);
    packet[15] = packet[21] = random(256);
    index += 12;    
    /** SOURCE_MAC **/

    for(int i=0;i<random(12,64);i++)
    {
        packet[index++] = random(255);
    }

    wifi_send_pkt_freedom(packet, index, 0);
    wdt_reset();
    delay(0);
}

