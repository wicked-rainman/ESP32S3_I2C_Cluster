#include "NetworkScan.h"
void NetworkScan(void *Summat) {
  int16_t NetworkCount;
  int16_t ctr;
  while(true) {
    NetworkCount = WiFi.scanNetworks(false,true,true);
    for (ctr = 0; ctr < NetworkCount; ctr++) SsidStorePut(WiFi.BSSID(ctr), (char *) WiFi.SSID(ctr).c_str(),WiFi.RSSI(ctr)); 
    vTaskDelay(SCAN_INTERVAL);
  }
}