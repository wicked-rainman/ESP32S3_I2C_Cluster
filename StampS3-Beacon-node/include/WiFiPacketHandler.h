#include <string.h>
#include "defines.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

extern SemaphoreHandle_t xShmem;
static wifi_country_t wifi_country = {.cc="GB", .schan = 1, .nchan = 13}; 

typedef struct {
  unsigned frame_ctrl:16;
  unsigned duration_id:16;
  uint8_t addr1[6]; /* receiver address */
  uint8_t addr2[6]; /* sender address */
  uint8_t addr3[6]; /* filtering address */
  unsigned sequence_ctrl:16;
  uint8_t addr4[6]; 
} wifi_ieee80211_mac_hdr_t;

typedef struct {
  wifi_ieee80211_mac_hdr_t hdr;
  uint8_t payload[0]; 
} wifi_ieee80211_packet_t;

extern uint8_t channelSequence[CHANNEL_COUNT];
extern uint8_t channelCount;
extern uint8_t level,channel ;
extern uint8_t BeaconStore[BEACON_CACHE_SIZE][BEACON_REC_SIZE];
extern uint8_t RingBuffPos;
void WiFiPacketHandler(void* buff, wifi_promiscuous_pkt_type_t type);