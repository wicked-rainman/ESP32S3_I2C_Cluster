#include "Concentrate.h"
bool Concentrate(uint8_t *newrec) {
    static int longctr=0;
    static int shortctr=0;
    static uint8_t shortMac[12];
    int ctr;

    //If Addr1 or Addr2 are not rolling macs, then search for them
    //by full mac address. If not found, then add them and return true
    
    if((!RollingMac(newrec[0])) || (!RollingMac(newrec[6]))) {
        for(ctr=0;ctr<DEDUPE_LONG_BUF_SIZE;ctr++) {
            if(!memcmp(newrec,DedupeBufLong[ctr],18)) {
                return false;
            }
            if(!memcmp(DedupeBufLong[ctr],"\0\0\0\0\0",5)) break;
        }
        memcpy(DedupeBufLong[longctr++],newrec,18);
        if(longctr>=DEDUPE_LONG_BUF_SIZE) {
            longctr=0;
        }
        return true;
    }
    else {
        // Addr1 or Addr2 are rolling macs or broadcast
        memcpy(shortMac,newrec+1,4);
        memcpy(shortMac+4,newrec+7,4);
        memcpy(shortMac+8,newrec+13,4);

        for(ctr=0;ctr<DEDUPE_SHORT_BUF_SIZE;ctr++) {
            if(!memcmp(shortMac,DedupeBufShort[ctr],12)) {
                return false;
            }
            if(!memcmp(DedupeBufShort[ctr],"\0\0\0\0\0",5)) break;
        }
        memcpy(DedupeBufShort[shortctr++],shortMac,12);
        if(shortctr>=DEDUPE_SHORT_BUF_SIZE) {
            shortctr=0;
        }
        return true;
    }
}
