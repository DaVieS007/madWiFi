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
byte RF = 0;
byte DIS = 0;
int autorun = 0;
String Blacklist;
String Beacons;
uint8_t channel = 0;

#define LED1 D0
#define LED2 D4
#define BTN 0

#include "utils.h"
#include "config.h"
#include "leds.h"
#include "madWifi_beacon.h"
#include "madWifi.h"
#include "ledBreathe.h"
#include "madWifi_deauth.h"
#include "madWifi_random.h"
#include "prompt.h"

void setup()
{

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);

    digitalWrite(LED1,LOW);
    digitalWrite(LED2,LOW);

    Serial.begin(115200);
    for(int i=1;i<200;i++)
    {
        Serial.println("");
    }

    
    Serial.println("Booting Up ...");
    // Promiscuous works only with station mode
    delay(1000);
    wifi_set_opmode(STATION_MODE);
    Serial.println("Wireless is in STATION_MODE");
    Serial.println("Press FLASH to format SPIFFS");
    delay(1000);
    if(digitalRead(BTN) == 0)
    {
        Serial.print("Formatting SPIFFS ...");
        SPIFFS.format();
        Serial.println("Done");
    }
    
    bool result = SPIFFS.begin();
    Serial.println("SPIFFS opened");
    Serial.println("Check for autorun ...");
    config_get("autorun");


    channel = 1;

    wifi_set_channel(channel);
    wifi_promiscuous_enable(0);
    wifi_set_promiscuous_rx_cb(promisc_cb);
    wifi_promiscuous_enable(1);
    wifi_set_opmode(STATION_MODE);

    
    Serial.println("Promiscous Mode Activated");
    Serial.printf("\n\n");
    Serial.printf("SDK version:%s\n", system_get_sdk_version());

    if(autorun != 0)
    {
        Serial.println("\nAutorun Delayed for 30 Seconds, enter stop or press FLASH to distrupt ...\n");
    }

    digitalWrite(LED1,HIGH);
    digitalWrite(LED2,HIGH);

}

int btn_trigger = 0;

void loop()
{
    byte _mad = MAD;
    led1_clear(500);
    led2_clear(500);

    if(trigger(1000,&btn_trigger))
    {
        if(digitalRead(BTN) == 0)
        {
            if(MAD == 1)
            {
                MAD = 0;
            }
            else
            {
                MAD = 1;
            }
        }
    }

    if(MAD == 1 && autorun > 0 && autorun + 30*1000 > millis())
    {
        _mad = 0;
    }

    if(RF == 1)
    {
        _mad = 1;
    }

    if(DIS == 1)
    {
        _mad = 1;
    }

    
    prompt();

    
    if(_mad == 0)
    {
        ledBreathe(LED1);  
    }
    else if(DIS == 1)
    {
        //led1_blink(20);
    }
    else
    {
        delay(50);
        madWifi_worker();
        madWifi_beacon();
    }
}

