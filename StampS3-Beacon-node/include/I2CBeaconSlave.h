#include <Wire.h>
#include <string.h>
#include "defines.h"
void I2CBeaconSlaveRequest();
extern uint8_t RingBuffPos;
extern uint8_t BeaconStore[][BEACON_REC_SIZE];
extern SemaphoreHandle_t xShmem;
