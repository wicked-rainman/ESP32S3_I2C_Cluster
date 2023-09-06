#include "I2CBeaconSlave.h"
void I2CBeaconSlaveRequest() {
    static uint8_t curpos;
    curpos = RingBuffPos;
    (bool) curpos ? curpos-- : curpos = (BEACON_CACHE_SIZE-1);
    Wire.write(BeaconStore[curpos],BEACON_REC_SIZE);
    xSemaphoreGiveFromISR(xShmem,NULL);
}