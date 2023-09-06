#include "load_stale_oui.h"
void load_stale_oui() {
    int k;
    oui_count = stale_oui_count;
    for(k=0;k<oui_count;k++) strncpy(oui_array[k],stale_oui_array[k],100);
}