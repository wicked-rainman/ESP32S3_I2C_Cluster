#include "LoadOuiTable.h"
//---------------------------------------------------------------
// Try and load the OUI table off the server filesystem.
// If this can't be done, then load an out of date (stale) OUI
//---------------------------------------------------------------
void LoadOuiTable(void *summat) {
    char input_line[200];
    WiFiServer server(180);
    server.begin();
    while(true) {
        WiFiClient client = server.available();
        if(client) {
            oui_count=0;
            while(client.connected()) {
                leds[0]=0x00000f;
                FastLED.show();
                memset(input_line,0,100);
                while(client.readBytesUntil('\n',input_line,100)) {
                    USBSerial.printf("Inserting: \"%s\"\n", input_line);
                    strncpy(oui_array[oui_count++],input_line,100);
                    memset(input_line,0,100);
                }
            }
            client.stop();
            leds[0]= 0x000000;
            FastLED.show();
        }
        vTaskDelay(100);
    }
}