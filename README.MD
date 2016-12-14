### This is our 'MadWifi' Project
Based upon Esressif ESP8266 Module

https://espressif.com/en/products/hardware/esp8266ex/overview

### Disclaimer
Blocking or/and disturbing Radio Frequency including IEE802.11 layer is a criminal depending on local-laws. 
Use with careful and **DO NOT** use as abusively.

We used to demonstration about WiFi weak points and stress test for our Wireless Infrastructure.

### Functionality
Some functionality forked from: https://github.com/RandDruid/esp8266-deauth

- WiFi AP Deauth (fast)
- WiFi Client Deauth
- WiFi Disortion Test
- WiFi **WPA2 Beacon** Frame Injection
- autorun, multi-configuration with **SPIFFS** Support
- Led Dimming (Playing with LED parallel) also known as LED Breathe
- Serial (USB-CDC) I/O

### Configuration
- Configuration only possible over USB CDC (Virt. Serial Port)
- We used Arduino IDE 1.6 with Serial Monitor


### Available Commands
````
 + show AP (Show APs)
 + show client (Show Clients)
 + add_beacon (Add Beacon to Broadcast)
 + del_beacon (Remove Beacon from Broadcast)
 + disortion on/off (Send Random Fragments)
 + beacons (List Beacons)
 + deny (MAC (BSSID))
 + allow (MAC (BSSID))
 + list (Show Settings)
 + start (Start Madness)
 + stop (Stop Madness)
 + clear all List Configs
 + config save [name/autorun] (Save Current Config)
 + config load [name/autorun] (Load Saved Config)
 + config del [name/autorun] (Del Saved Config)
 + config list (List Configs)
````

### Note
*Espressif Disabled Control Frame Injection above SDK 1.3.0 so you need SDK 1.3.0 to get all functionality work. We are not help you to do that since we are not a toy factory for kids we assume if you interested this project you have enough talent and skills to solve this problem.*

