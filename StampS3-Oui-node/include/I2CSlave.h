#include <Wire.h>
#include <FastLED.h>
#include "defines.h"
extern int oui_count;
extern char oui_array[][50];
extern uint8_t MacOui[3];
extern char DevName[30];
extern CRGB leds[NUM_LEDS];
void I2CSlaveRequest();
void I2CSlaveReceive(int);
void FindMacDev(uint8_t *);