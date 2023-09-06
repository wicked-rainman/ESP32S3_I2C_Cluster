#include <WiFi.h>
#include <FastLED.h>
#include "defines.h"
#define MAX_WAIT_TIME 5
void LoadOuiTable(void *);
extern char *password;
extern char *ssid;
extern int oui_count;
extern char oui_array [][50];
extern CRGB leds[NUM_LEDS];
