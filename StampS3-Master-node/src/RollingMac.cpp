#include "RollingMac.h"
bool RollingMac(uint8_t oui) {
    char Octet[3];
    sprintf(Octet,"%02x",oui);
    if(Octet[1] == '2') return true;
    if(Octet[1] == '6') return true;
    if(Octet[1] == 'a') return true;
    if(Octet[1] == 'e') return true;
    return false;
}