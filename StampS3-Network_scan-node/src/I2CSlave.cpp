#include <Wire.h>
extern char ReturnVal[35];
extern void SsidStoreGet(uint8_t *, uint8_t, char *);
void I2CSlaveReceive(int);
void I2CSlaveRequest();
extern SemaphoreHandle_t xShmem;
//-------------------------------------------------------------------------------
void I2CSlaveReceive(int fred) {
    char ReceiveString[10];
    uint8_t bssid[6];
    uint8_t mode;
    int ctr=0;
    while(Wire.available()) ReceiveString[ctr++] = Wire.read();
    mode=ReceiveString[0];
    memcpy(bssid, ReceiveString+1,6);
    SsidStoreGet(bssid,mode,ReturnVal);
    strcat(ReturnVal,"\n");
    xSemaphoreGiveFromISR(xShmem,NULL);
}
//-------------------------------------------------------------------------------
void I2CSlaveRequest() {
    Wire.write(ReturnVal);
}