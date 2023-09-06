#include <FastLED.h>
#include "defines.h"
#include "I2CSlave.h"
#include "FoundMacDev.h"
#include "OuiIndex.h"
#include "LoadOuiTable.h"
#include "load_stale_oui.h"
#include "WiFiConnect.h"
uint8_t MacOui[3];
char DevName[30];
CRGB leds[NUM_LEDS];
char* ssid     = "SomeNetwork";
char* password = "SomePassword";
char oui_array [1000][50];
int oui_count=0;
