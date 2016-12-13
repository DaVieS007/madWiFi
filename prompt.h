char sCmd[255];
byte sCmd_idx;
byte inited = 0;

void prompt()
 {
    if(inited == 0)
    {
        inited = 1;
        Serial.println("Serial Console initiated, type 'help'");
        Serial.print(">");
    }
    
    if (Serial.available() > 0) 
    {
        sCmd[sCmd_idx] = Serial.read();
        
        if(sCmd[sCmd_idx] == '\n')
        {
          sCmd[sCmd_idx + 1] = '\0';
          String buff = String(sCmd);
          buff.replace("\r","");
          buff.replace("\n","");
          
          Serial.println("Command: '" + buff + "'");  
          Serial.println("");  
          Serial.println("");  
          sCmd_idx = 0;

          if(StringIndex(buff, " ", 0) == "show")
          {
              if(StringIndex(buff, " ", 1) == "AP")
              {                
                  Serial.printf("[%32s] | ", "SSID");
                  Serial.printf("%12s | ", "BSSID");
                  Serial.printf("%2s | ", "CH");
                  Serial.printf("%4s | \r\n", "RSSI");
                  for(int i=1;i<32+12+2+4+3+3+3+3+2;i++)
                  {
                      Serial.print("-");
                  }
                  Serial.println("");

                  for(int i=0;i<aps_known_count;i++)
                  {
                      print_beacon(aps_known[i]);
                  }
              }
              else if(StringIndex(buff, " ", 1) == "client")
              {
                  Serial.printf("[%32s] | ", "SSID");
                  Serial.printf("%12s | ", "BSSID");
                  Serial.printf("%2s | ", "CH");
                  Serial.printf("%4s | \r\n", "RSSI");
                  for(int i=1;i<32+12+2+4+3+3+3+3+2;i++)
                  {
                      Serial.print("-");
                  }
                  Serial.println("");
                  
                  for(int i=0;i<clients_known_count;i++)
                  {
                      print_client(clients_known[i]);
                  }
              }
          }
          else if(buff == "help")
          {
              Serial.println("");          
              Serial.println("");          
              Serial.println("*** madWiFi Project by nPulse.net ***");          
              Serial.println("");          
              Serial.println("");          
              Serial.println(" * Available Commands:");          
              Serial.println(" + show AP (Show APs)");          
              Serial.println(" + show client (Show Clients)");          
              Serial.println(" + add_beacon (Add Beacon to Broadcast)");          
              Serial.println(" + del_beacon (Remove Beacon from Broadcast)");          
//              Serial.println(" + random_fragment start/stop (Send Random Fragments)");
              Serial.println(" + disortion on/off (Send Random Fragments)");
              Serial.println(" + beacons (List Beacons)");          
              Serial.println(" + deny (MAC (BSSID))");        
              Serial.println(" + allow (MAC (BSSID))");        
              Serial.println(" + list (Show Settings)");        
              Serial.println(" + start (Start Madness)");        
              Serial.println(" + stop (Stop Madness)");        
              Serial.println(" + clear all List Configs");        
              Serial.println(" + config save [name/autorun] (Save Current Config)");        
              Serial.println(" + config load [name/autorun] (Load Saved Config)");        
              Serial.println(" + config del [name/autorun] (Del Saved Config)");        
              Serial.println(" + config list (List Configs)");        
          }
          else if(StringIndex(buff, " ", 0) == "config")
          {
                if(StringIndex(buff, " ", 1) == "list")
                {
                    config_list();
                }
                else if(StringIndex(buff, " ", 1) == "save" && StringIndex(buff, " ", 2).length() > 0)
                {
                    config_save(StringIndex(buff, " ", 2));
                }
                else if(StringIndex(buff, " ", 1) == "load" && StringIndex(buff, " ", 2).length() > 0)
                {
                    config_get(StringIndex(buff, " ", 2));
                }
                else if(StringIndex(buff, " ", 1) == "del" && StringIndex(buff, " ", 2).length() > 0)
                {
                    config_del(StringIndex(buff, " ", 2));
                }
          }
          else if(StringIndex(buff, " ", 0) == "random_fragment")
          {
                if(StringIndex(buff, " ", 1) == "start")
                {
                    Serial.println("Random Fragment Started");
                    RF = 1;
                }
                else if(StringIndex(buff, " ", 1) == "stop")
                {
                    Serial.println("Random Fragment Stopped");
                    RF = 0;
                }
          }
          else if(StringIndex(buff, " ", 0) == "disortion")
          {
                if(StringIndex(buff, " ", 1) == "on")
                {
                    Serial.println("Random Disortion Started");
                    DIS = 1;
                }
                else if(StringIndex(buff, " ", 1) == "off")
                {
                    Serial.println("Random Disortion Stopped");
                    DIS = 0;
                }
          }
          else if(StringIndex(buff, " ", 0) == "deny" && StringIndex(buff, " ", 1).length() > 0)
          {
              if(Blacklist.indexOf(" " + StringIndex(buff, " ", 1)) < 0)
              {
                  Blacklist += " " + StringIndex(buff, " ", 1);
                  Serial.println(StringIndex(buff, " ", 1) + " added to the list.");
              }
              else
              {
                  Serial.println(StringIndex(buff, " ", 1) + " is already on the list.");
              }
          }
          else if(StringIndex(buff, " ", 0) == "allow" && StringIndex(buff, " ", 1).length() > 0)
          {
              if(Blacklist.indexOf(" " + StringIndex(buff, " ", 1)) >= 0)
              {
                  Blacklist.replace(" " + StringIndex(buff, " ", 1),"");
                  Serial.println(StringIndex(buff, " ", 1) + " removed from the list.");
              }
              else
              {
                  Serial.println(StringIndex(buff, " ", 1) + " not found on the list.");
              }
          }
          else if(StringIndex(buff, " ", 0) == "add_beacon" && StringIndex(buff, " ", 1).length() > 0)
          {
              if(Beacons.indexOf(StringIndex(buff, " ", 1) + " ") < 0)
              {
                  Beacons += StringIndex(buff, " ", 1) + " ";
                  Serial.println(StringIndex(buff, " ", 1) + " added to the list.");
              }
              else
              {
                  Serial.println(StringIndex(buff, " ", 1) + " is already on the list.");
              }
          }
          else if(StringIndex(buff, " ", 0) == "del_beacon" && StringIndex(buff, " ", 1).length() > 0)
          {
              if(Beacons.indexOf(StringIndex(buff, " ", 1) + " ") >= 0)
              {
                  Beacons.replace(StringIndex(buff, " ", 1) + " ","");
                  Serial.println(StringIndex(buff, " ", 1) + " removed from the list.");
              }
              else
              {
                  Serial.println(StringIndex(buff, " ", 1) + " not found on the list.");
              }
          }
          else if(buff == "start")
          {
              MAD = 1;
          }
          else if(buff == "stop")
          {
              MAD = 0;
              autorun = 0;
          }
          else if(buff == "clear")
          {
              MAD = 0;
              Beacons = "";
              Blacklist = "";
              Serial.println("Lists Cleared");
          }
          else if(buff == "list")
          {
              Serial.println("BlackList: " + Blacklist);
              Serial.println("Beacons: " + Beacons);
          }
          
          Serial.println("");
          Serial.print(">");
        }
        else
        {
            sCmd_idx++;
          
        }
    }
}
