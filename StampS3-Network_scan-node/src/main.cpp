#include "main.h"
//-------------------------------------------------------------------------------------------
void setup() {
  USBSerial.begin(115200);
  vTaskDelay(1200);
  xShmem = xSemaphoreCreateBinary();
  xUsb = xSemaphoreCreateMutex();
  xSemaphoreGive(xShmem);
  xSemaphoreGive(xUsb);
  SSID_storeUsed=0;
  pinMode(BUTTON,0x01);
  FastLED.addLeds<WS2812,LED, GRB>(leds, NUM_LEDS);
  for(int k=0;k<BLUE_LIST_SIZE;k++) SsidStorePut(BLUE_LIST[k],"BLUE", (int32_t) 0);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Wire.setPins(13,15);
  Wire.begin(0x56);
  Wire.onRequest(I2CSlaveRequest);
  Wire.onReceive(I2CSlaveReceive);
  xTaskCreatePinnedToCore(&NetworkScan,"Scan",2048,NULL,5,NULL,1);
  xTaskCreatePinnedToCore(&RollToFix,"MacFix",2048,NULL,5,NULL,0);
}
//-------------------------------------------------------------------------------------------
void loop() {
  vTaskDelay(50);
}