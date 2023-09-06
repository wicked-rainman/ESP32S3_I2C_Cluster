#include <WiFi.h>
#include <FastLED.h>
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "defines.h"
#include "I2CBeaconSlave.h"
#include "WiFiPacketHandler.h"
SemaphoreHandle_t xShmem;
CRGB leds[NUM_LEDS];
uint8_t led_ih = 0;
uint8_t led_status = 0;
static void wifi_sniffer_packet_handler(void *buff, wifi_promiscuous_pkt_type_t type);
uint8_t level = 0, channel = 0;
uint8_t BeaconStore[BEACON_CACHE_SIZE][BEACON_REC_SIZE];
uint8_t RingBuffPos=0;
uint8_t channelSequence[CHANNEL_COUNT]={1,6,11,3,4,1,6,11,8,9,1,6,11,2,7,1,6,11,5,10};
uint8_t channelCount=CHANNEL_COUNT;