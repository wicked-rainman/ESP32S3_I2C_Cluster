#include "FoundMacDev.h"
void FindMacDev(uint8_t *MacOui) {
    static char MacStr[8]; 
    int i=0;

    snprintf(MacStr,7,"%02x%02x%02x",MacOui[0],MacOui[1],MacOui[2]);
    for(i=0;i < oui_count;i++) {
        if(!strncmp(MacStr, oui_array[i],6)) {
            strncpy(DevName,oui_array[i]+7,30);
            strcat(DevName,"\n");
            return;
        }
    }
    strncpy(DevName,"OUI_Unk\n",(size_t) 9);
}
