#include "main.h"

void setup() {
  Wire.setPins(13,15);
  Wire.begin(0x57);
  USBSerial.begin(115200);
  vTaskDelay(1200);
  FastLED.addLeds<WS2812,LED, GRB>(leds, NUM_LEDS);
  WiFiConnect(ssid,password);
  load_stale_oui();
  Wire.onRequest(I2CSlaveRequest);
  Wire.onReceive(I2CSlaveReceive);
  xTaskCreatePinnedToCore(LoadOuiTable,"LoadOuiTable", 5000, NULL,3,NULL,0);
}

void loop() { }