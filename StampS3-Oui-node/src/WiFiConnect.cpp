#include "WiFiConnect.h"
void WiFiConnect(char * ssid,char *password) {
    WiFi.begin(ssid,password);
    while(WiFi.status() != WL_CONNECTED) vTaskDelay(500);
    USBSerial.printf("This is OUI. IP=");
    USBSerial.print(WiFi.localIP());
    USBSerial.printf("\n");
    leds[0] = 0x0f0000;
    FastLED.show();
}