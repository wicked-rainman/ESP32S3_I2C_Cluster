#include "FetchNextBeacon.h"
bool FetchNextBeacon() {
    static uint8_t lastBeacon=0;
    if(Wire.requestFrom(0x55,20) == 20) Wire.readBytes(buf,20);
    else return false;              //Didn't receieve 20 bytes, so not a valid beacon
    if(lastBeacon != buf[19]) {     //New beacon 
        lastBeacon = buf[19];
        return true;
    }
    return false;
}