#include "ResolveDevice.h"
void ResolveDevice(uint8_t *mac, char *devname) {
    int i=0;
    int j;
    //Send the request
    Wire.beginTransmission(0x57); 
    Wire.write(mac,3);        
    Wire.endTransmission(); 
    Wire.requestFrom(0x57, 32);
    j=0;
    //Wait for a reply
    while(!Wire.available()) {
        vTaskDelay(10/portTICK_RATE_MS);
        j++;
        if(j>100) {
            vTaskDelay(100/portTICK_RATE_MS);
            Wire.flush();
            Wire.beginTransmission(0x57); 
            Wire.write( mac,3);        
            Wire.endTransmission();    
            Wire.requestFrom(0x57, 30);
            j=0;
        }
    }
    //Got a reply
    while(Wire.available()){
        devname[i++] = Wire.read();
        if(devname[i - 1] =='\n') devname[i-1]=0x00;
    }
}
