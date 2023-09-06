#include <FastLED.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include "defines.h"
#include "ResolveSSID.h"
#include "ResolveDevice.h"
#include "FetchNextBeacon.h"
#include "RollingMac.h"
#include "Concentrate.h"
#include "udp_utils.h"
char NetworkMap[20];
char DeviceMac[20];
char packet[13];
uint8_t buf[20];
uint8_t DedupeBufShort[DEDUPE_SHORT_BUF_SIZE][12];
uint8_t DedupeBufLong[DEDUPE_LONG_BUF_SIZE][18];
int LastMacRec = 0;
int StoreUsed = 0;
char* ssid     = "SomeNetwork";
char* password = "SomePassword";
WiFiUDP beaconPort;
IPAddress SendIP(192, 168, 1, 255);
CRGB leds[NUM_LEDS];
