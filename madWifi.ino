#include "FS.h"

// Expose Espressif SDK functionality - wrapped in ifdef so that it still
// compiles on other platforms
#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif

#include <ESP8266WiFi.h>

int madness = 0;
byte MAD = 0;
int autorun = 0;
String Blacklist;
String Beacons;

#include "utils.h"
#include "config.h"
#include "madWifi.h"
#include "ledBreathe.h"
#include "madWifi_deauth.h"
#include "madWifi_beacon.h"
#include "prompt.h"

void setup()
{
    Serial.begin(115200);
    for(int i=1;i<200;i++)
    {
        Serial.println("");
    }

    Serial.println("Bootup Completed .");
    // Promiscuous works only with station mode
    wifi_set_opmode(STATION_MODE);
    Serial.println("Wireless is in STATION_MODE");

    bool result = SPIFFS.begin();
    Serial.println("SPIFFS opened");
    Serial.println("Check for autorun ...");
    config_get("autorun");


    channel = 1;

    wifi_set_channel(channel);
    wifi_promiscuous_enable(0);
    wifi_set_promiscuous_rx_cb(promisc_cb);
    wifi_promiscuous_enable(1);
    Serial.println("Promiscous Mode Activated");
    Serial.printf("\n\n");
    Serial.printf("SDK version:%s\n", system_get_sdk_version());

    if(autorun != 0)
    {
        Serial.println("\nAutorun Delayed for 30 Seconds, stop to distrupt ...\n");
    }
    
}

void loop()
{
    byte _mad = MAD;

    if(MAD == 1 && autorun > 0 && autorun + 30*1000 > millis())
    {
        _mad = 0;
    }
    
    prompt();
    if(_mad == 0)
    {
        ledBreathe(D0);  
    }
    else
    {
        digitalWrite(D0, LOW);
        madWifi_worker();
        madWifi_beacon();
        delay(100);
        digitalWrite(D0,HIGH);
        delay(100);
    }
}

