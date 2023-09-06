#include "main.h"

//-------------------------------------------------------------------------------------------
void setup() {
  USBSerial.begin(115200);
  USBSerial.flush();
  vTaskDelay(1200);
  FastLED.addLeds<WS2812,LED, GRB>(leds, NUM_LEDS);
  xShmem = xSemaphoreCreateBinary();
  xSemaphoreGive(xShmem);
  nvs_flash_init();
  tcpip_adapter_init();
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);
  esp_wifi_set_country(&wifi_country); 
  esp_wifi_set_storage(WIFI_STORAGE_RAM);
  esp_wifi_set_mode(WIFI_MODE_NULL);
  esp_wifi_start();
  esp_wifi_set_promiscuous(true);
  USBSerial.printf("\nThis is Beacon (%s)\n\n",WiFi.macAddress().c_str());
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Wire.setPins(13,15);
  Wire.begin(0x55);
  Wire.onRequest(I2CBeaconSlaveRequest);
  esp_wifi_set_promiscuous_rx_cb(&WiFiPacketHandler);
}
//-------------------------------------------------------------------------------------------
void loop() {
  vTaskDelay(WIFI_CHANNEL_SWITCH_INTERVAL);;
  esp_wifi_set_channel(channelSequence[channel++], WIFI_SECOND_CHAN_NONE);
  if(channel == CHANNEL_COUNT) channel = 0;
}