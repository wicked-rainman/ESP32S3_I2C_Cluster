#include <FastLED.h>
#include "defines.h"
#include "Structures.h"
extern int SSID_storeUsed;
struct Store storeArray[MAX_SSID_STORE_SIZE];
extern void CharReplace(char *, char, char);
extern SemaphoreHandle_t xShmem;
extern SemaphoreHandle_t xUsb;
extern CRGB leds[NUM_LEDS];
extern bool RollingMac(uint8_t);
void SsidStoreGet(uint8_t *, uint8_t, char *);
bool SsidStorePut(uint8_t *, char *, int32_t);
bool SsidStoreFind(uint8_t *);
void RollToFix(void *); 