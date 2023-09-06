#include "ResolveSSID.h"
bool ResolveSSID(uint8_t *bssid,char *ssid) {
    //----------------------------------------------------------------------------//
    // The SSID has a one-to-one relationship with the BSSID (Mac).               //
    // Some APs advertise three networks. Often one is hidden (No SSID) another   //
    // is a ISP shared access SSID and the third is private (the customer SSID    //
    //                                                                            //
    // The hidden and shared networks often have rolling MAC addresses            //
    // that are based on a variation of the customer network MAC. Sometimes the   //
    // first of the 6 bytes is different and sometimes it's the first and last    //
    // bytes that vary.                                                           //
    // The network scanner can associate the rolling macs with the customer SSID  //
    // if required (It learns over time). This routine can request the associated //
    // SSID, or the one associated with the detected MAC.                         //
    // To choos the association, use SSID_MODE_ASSOC. To obtain the origional     //
    // SSID use SSID_MODE_SSID.                                                   //
    //----------------------------------------------------------------------------//
    int i,j;
    uint8_t mbssid[7];
    mbssid[0] = SSID_MODE_ASSOC;
    memcpy(mbssid+1,bssid,6);
    Wire.beginTransmission(0x56); 
    Wire.write(mbssid,7);        
    Wire.endTransmission();    
    Wire.requestFrom(0x56, 200);
    j=0;
    while(!Wire.available()) {
        vTaskDelay(10/portTICK_RATE_MS);
        j++;
        if(j>100) {
            //USBSerial.printf("Master:I2c retring fetch for SSID\n");
            vTaskDelay(100/portTICK_RATE_MS);
            Wire.flush();
            Wire.beginTransmission(0x56); 
            Wire.write(mbssid,7);        
            Wire.endTransmission();    
            Wire.requestFrom(0x56, 200);
            j=0;
        }
    }
    i=0;
    while(Wire.available()){
        ssid[i++] = Wire.read();
        if(ssid[i - 1] =='\n') ssid[i-1]=0x00;
    }
    if(strncmp(ssid,"NET_Unk",7)) return false;
    return true;

}