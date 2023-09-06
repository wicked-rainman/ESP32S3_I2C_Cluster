#include "I2CSlave.h"

//-------------------------------------------------------------------------------
void I2CSlaveReceive(int fred) {
    int ctr;
    while(Wire.available()) MacOui[ctr++] = Wire.read();
    leds[0] = 0x000f00;
    FastLED.show();
    FindMacDev(MacOui);
}
//-------------------------------------------------------------------------------
void I2CSlaveRequest() {
    Wire.write(DevName);
    leds[0] = 0x000000;
    FastLED.show();
}