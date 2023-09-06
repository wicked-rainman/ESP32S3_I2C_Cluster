#include "store.h"
//----------------------------------------------------------
void SsidStoreGet(uint8_t *index, uint8_t mode, char *retval) {
    int storeCtr;
    uint8_t offset=0;
    size_t len=6;
    leds[0] = 0x000f00;
    FastLED.show();
    if(RollingMac(index[0])) {
        offset=1;
        len=4;
    }
    for(storeCtr=0;storeCtr<SSID_storeUsed;storeCtr++) {
        if (!memcmp(storeArray[storeCtr].MacAddress+offset, index+offset,len)) {
            switch (mode) {
                case 0 : {  //SSID will be returned
                    strcpy(retval,storeArray[storeCtr].Ssid);
                    leds[0] = 0x000000;
                    FastLED.show();
                    return;
                }
                case 1 : {  //SSID association will be returned
                    strcpy(retval,storeArray[storeCtr].Ssid_Assoc);
                    leds[0] = 0x000000;
                    FastLED.show();
                    return;
                }
                case 2 : {  //RSSI will be returned
                    sprintf(retval,"%d",storeArray[storeCtr].Rssi);
                    leds[0] = 0x000000;
                    FastLED.show();
                    return;
                }
                default : {
                    sprintf(retval,"Invalid");
                    leds[0] = 0x000000;
                    FastLED.show();
                    return;
                }

            }
        }
    }
    if(mode == 2) strcpy(retval,"0");
    else sprintf(retval,"NET_Unk");
    leds[0] = 0x000000;
    FastLED.show();
    return;
}
//---------------------------------------------------------
bool SsidStorePut(uint8_t *index, char *val, int32_t rssival) {
    if(!SsidStoreFind(index)) {
      leds[0] = 0x00000f;
      FastLED.show();
        if(SSID_storeUsed <MAX_SSID_STORE_SIZE) {
            if (xSemaphoreTake(xShmem, portMAX_DELAY)==pdTRUE) {
                memcpy(storeArray[SSID_storeUsed].MacAddress, index, (size_t) 6);
                CharReplace(val,' ','_');
                memcpy(storeArray[SSID_storeUsed].Ssid, val,strlen(val));
                storeArray[SSID_storeUsed].Rssi = rssival;
                if(RollingMac(index[0])) storeArray[SSID_storeUsed].rolling = 'r';
                else storeArray[SSID_storeUsed].rolling='f';
                strncpy(storeArray[SSID_storeUsed].Ssid_Assoc,storeArray[SSID_storeUsed].Ssid,MAX_SSID_LENGTH);
                if(xSemaphoreTake(xUsb,portMAX_DELAY) == pdTRUE) {
                    USBSerial.printf("%3d Added %02x:%02x:%02x:%02x:%02x:%02x (%c) %d \"%s\" \"%s\"\n",
                        SSID_storeUsed,storeArray[SSID_storeUsed].MacAddress[0],storeArray[SSID_storeUsed].MacAddress[1],storeArray[SSID_storeUsed].MacAddress[2],
                        storeArray[SSID_storeUsed].MacAddress[3],storeArray[SSID_storeUsed].MacAddress[4],storeArray[SSID_storeUsed].MacAddress[5],
                        storeArray[SSID_storeUsed].rolling,storeArray[SSID_storeUsed].Rssi,
                        storeArray[SSID_storeUsed].Ssid_Assoc,storeArray[SSID_storeUsed].Ssid);
                    xSemaphoreGive(xUsb);
                }
                SSID_storeUsed++;
                xSemaphoreGive(xShmem);
                leds[0] = 0x000000;
                FastLED.show();
                return true;
            }
        }
        leds[0] = 0x000000;
        FastLED.show();
    }
    return false;
}
//---------------------------------------------------------
bool SsidStoreFind(uint8_t *index) {
    int storeCtr;
    for(storeCtr=0;storeCtr<SSID_storeUsed;storeCtr++) {
        if (!memcmp(storeArray[storeCtr].MacAddress, index,6)) return true;
    }
    return false;
}
//---------------------------------------------------------
void RollToFix(void *SummatElse) {
//
// Rolling MAC addresses are sometimes related to a fixed MAC
// (for example, with some commercial APs).
// Such rolling MAC addresses might turn up before or after a fixed 
// associated MAC. This causes a boundary clash when making the
// association. This routine fixes that issue whenever it is called, but
// only when a fixed association MAC has already been stored.
// For any associated rolling mac, status is changed from 'r' to
// 'e' so that the association can be detected in follow on
// processing.
//
int ctr;
int innerctr;
  while(true) {
    if(xSemaphoreTake(xShmem,portMAX_DELAY)==pdTRUE) {
      for(ctr=0;ctr<SSID_storeUsed;ctr++){
        if(storeArray[ctr].rolling == 'r') {
          for(innerctr=0;innerctr<SSID_storeUsed;innerctr++) {
            if(storeArray[innerctr].rolling =='f') {
              if(!memcmp(storeArray[ctr].MacAddress+1, storeArray[innerctr].MacAddress+1,4)) {
                strncpy(storeArray[ctr].Ssid_Assoc, storeArray[innerctr].Ssid,MAX_SSID_LENGTH);
                storeArray[ctr].rolling = 'e';
                if(xSemaphoreTake(xUsb,portMAX_DELAY)==pdTRUE) {
                  USBSerial.printf("%3d Edit  %02x:%02x:%02x:%02x:%02x:%02x (%c) %d \"%s\" \"%s\"\n",
                    ctr,
                    storeArray[ctr].MacAddress[0],storeArray[ctr].MacAddress[1],storeArray[ctr].MacAddress[2],
                    storeArray[ctr].MacAddress[3],storeArray[ctr].MacAddress[4],storeArray[ctr].MacAddress[5],
                    storeArray[ctr].rolling, storeArray[ctr].Rssi,
                    storeArray[ctr].Ssid_Assoc,storeArray[ctr].Ssid);
                  xSemaphoreGive(xUsb);
                }
              }
            }
          }
        }
      }
      xSemaphoreGive(xShmem);
    }
    vTaskDelay(ROLL_FIX_DELAY);
  } 
}