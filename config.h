void config_get(String name)
{
    String line;
    Serial.print("Loading Configuration: '" + name + "'");      
    name = "/config/" + name;
  
    File f = SPIFFS.open(name.c_str(), "r");
    if (!f) 
    {
        Serial.println(" Failed");      
        return;
    }

    Blacklist = f.readStringUntil('\n');
    Beacons = f.readStringUntil('\n');
    if(f.readStringUntil('\n') == "DIS")
    {
        DIS = 1;
    }

    if(name == "/config/autorun")
    {
        MAD = 1;
        autorun = millis();
    }
    f.close();

    Serial.println(" Success");
    
}

void config_save(String name)
{
    String line;
    Serial.print("Saving Configuration: '" + name + "'");      
    name = "/config/" + name;
    File f = SPIFFS.open(name.c_str(), "w");
    if (!f) 
    {
        Serial.println(" Failed");      
        return;
    }

    f.println(Blacklist.c_str());
    f.println(Beacons.c_str());
    if(DIS == 1)
    {
        f.println("DIS");
    }

    f.close();
    Serial.println(" Success");
    
}

void config_del(String name)
{
    Serial.print("Deleting Configuration: '" + name + "'");
    name = "/config/" + name;
    File f = SPIFFS.open(name.c_str(), "w");
    if (!f) 
    {
        Serial.println(" Failed");      
        return;
    }

    f.close();
    SPIFFS.remove(name);
    Serial.println(" Success");
}

void config_list()
{

    Dir dir = SPIFFS.openDir("/config");
    while (dir.next()) 
    {
        Serial.println("Avaliable Configs: ");
        Serial.println(" + " + dir.fileName());
    }

}

