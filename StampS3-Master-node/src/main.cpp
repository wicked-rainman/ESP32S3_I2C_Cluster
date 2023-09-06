#include "main.h"

void setup() {
  bool error=false;
  bool toggle=true;
  pinMode(PIN_BUTTON, INPUT);
  FastLED.addLeds<WS2812,LED, GRB>(leds, NUM_LEDS);
  USBSerial.begin(115200);
  vTaskDelay(1200); //This is how long Espressif seem to think the USB serial port takes to init.

  //
  // This is the I2C Master.                                                      
  // ------------------------                                                     
  // It's function is as follows:                                                 
  //                                                                              
  // 1. Make sure that the three cluster node slaves can be found                 
  //    on the I2C bus.                                                           
  //                                                                              
  // 2.Request the latest beacon packet via I2c from the Beacon node.             
  //  There is no guarantee that every beacon packet will be processed, but
  //  they are extracted from a circular buffer on the beacon node end and in 
  //  practice, very little(if any) seem to get lost.
  //
  // 3.Extract the three Mac addresses from the beacon packet
  //  and see if all three combined (18 bytes) have been seen before.
  //  We are only intrested in TA, so if it's a duplicate just ignore it.
  //
  // 4.For each of the first three Mac addresses in the beacon, check to
  // see if the address is rolling. If it is, mark it as such.
  // If it's not a rolling mac, ask the OUI node to look up the vendor,
  // then ask the network scanning node if there is an associated SSID.
  //
  // 5. Output all that gleamed information via the serial port and
  // as a broadcast on UDP port 5128.
  //
  USBSerial.printf("This is the master node\n");
  Wire.setPins(13,15);
  Wire.begin();
  vTaskDelay(5000);
  Wire.beginTransmission(0x55);
  if(!Wire.endTransmission()) USBSerial.printf("Found I2c device: 0x55\n");
  else error=true;
  Wire.beginTransmission(0x56);
  if(!Wire.endTransmission()) USBSerial.printf("Found I2c device: 0x56\n");
  else error=true;
  Wire.beginTransmission(0x57);
  if(!Wire.endTransmission()) USBSerial.printf("Found I2c device: 0x57\n");
  else error=true;
  if(error) {
    //----------------------------------------------------------------------//
    // Could not contact the three nodes, so loop forever flashing the led  //
    //----------------------------------------------------------------------//
    while(true) {
      toggle = !toggle;
      if(toggle) leds[0] = 0xff0000;
      else leds[0] = 0x000000;   
      FastLED.show();
      vTaskDelay(200);
    }
  }
  //
  // Required nodes were found.   
  // Start up WiFi connection, waiting forever if it's not there    
  //
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED) {
      leds[0] = 0x0000ff;
      FastLED.show();
      vTaskDelay(200);
      leds[0] = 0x000000;
      FastLED.show();
      vTaskDelay(200);
  }
  //
  // Open up the UDP broadcast port    
  //
  beaconPort.begin(UDPBEACON_PORT);

//
// Wait for a button press before running. This gives a chance for a new OUI table   
// to be NC'd up to the OUI node and allows for the Network scanner to learn for      
// a while. The longer the time delay before pressing the button the better. This    
// is because the scanner performs on-the-fly editing of SSIDs to resolve            
// rolling AP mac addresses. In practice, one or two mins before activation          
// seems a good time to chill.                                                       
//
  leds[0] = 0x0f0000;
  FastLED.show();
  while(1) {
    if(!digitalRead(PIN_BUTTON)) {
      vTaskDelay(20);
      if(!digitalRead(PIN_BUTTON)) {
        leds[0] = 0x000000;
        FastLED.show();
        vTaskDelay(2000);
        break;
      }
    }
  }
  //
  // Initialise the two ring buffers used by the concentrator function (Dedupes records based   
  // on full or rolling mac addresses). For rolling macs, only the central 4 bytes are stored   
  // (They get put in the short buffer). Full macs get stuffed into the long buffer.            
  // If either get's found, then the beacon records are dropped as "seen before"                
  //
  memset(DedupeBufLong,0,DEDUPE_LONG_BUF_SIZE);
  memset(DedupeBufShort,0,DEDUPE_SHORT_BUF_SIZE);
}
//-----------------------------------------------------------------------------

void loop() {
  static uint8_t Addr1[6];
  static uint8_t Addr2[6];
  static uint8_t Addr3[6];
  static char Addr1Oui[40];
  static char Addr2Oui[40];
  static char Addr3Oui[40];
  static char SsidStr1[200];
  static char SsidStr2[200];
  static char SsidStr3[200];
  static uint8_t ChannelNo;
  static uint8_t SequenceNo;
  static uint8_t BroadcastOui[3]={255,255,255};
  char beaconStr[BEACON_RECORD_SIZE];


  //If a new beacon packet is available
  if(FetchNextBeacon()) {
    // If we've not seen the three combined Mac addresses before
    if(Concentrate(buf)) {
      leds[0] = 0x000f00;
      FastLED.show();
      memcpy(Addr1,buf,6);
      memcpy(Addr2,buf+6,6);
      memcpy(Addr3,buf+12,6);
      ChannelNo=buf[18];
      SequenceNo=buf[19];

      //
      //  Deal with Addr1 (Often broadcast)                               
      //
      
      if (!memcmp(Addr1,BroadcastOui,3)) {  //If it's a broadcast mac
        strcpy(Addr1Oui,"Bcast");           //Don't look up the OUI
        strcpy(SsidStr1,"NET_Unk");         //Don't look up the SSID
      }
      else if(RollingMac(Addr1[0])) {       //If it's a rolling mac  
        strcpy(Addr1Oui,"Roll");            //Don't look up the OUI, set it to Roll
        ResolveSSID(Addr1,SsidStr1);      //Look for previous assoc with an SSID
      }
      else {                                //It's a normal mac
        ResolveDevice(Addr1,Addr1Oui);      //Look up the OUI
        ResolveSSID(Addr1,SsidStr1);      //Look up the SSID
      }

      //
      //  Deal with Addr2                                                
      //

      if(RollingMac(Addr2[0])) {            //If it's a rolling mac (no broadcasts in Addr2)
        strcpy(Addr2Oui,"Roll");            //Set OUI to Roll
        ResolveSSID(Addr2,SsidStr2);      //Look for an SSID match
      }
      else {                                // It's a normal mac.
        ResolveDevice(Addr2,Addr2Oui);      //Look up the OUI
        ResolveSSID(Addr2,SsidStr2);      //Look up the SSID
      }

      //
      //  Deal with Addr3 if it's not the same as Addr2                   
      //

      if(memcmp(Addr2,Addr3,6)) {             //If Addr2 is different from Addr3
        if (!memcmp(Addr3,BroadcastOui,3)) {  //If it's a broadcast mac
          strcpy(Addr3Oui,"Bcast");           //Set OUI to roll
          strcpy(SsidStr3,"NET_Unk");         //Set SSID to Unknown
        }
        else if(RollingMac(Addr1[0])) {       //If it's a rolling mac  
          strcpy(Addr3Oui,"Roll");            //Set OUI to roll
          ResolveSSID(Addr3,SsidStr3);      //Look up SSID
        }
        else {                                //It's a normal mac
          ResolveDevice(Addr3,Addr3Oui);      //Look up OUI
          ResolveSSID(Addr3,SsidStr3);      //Look up SSID
        }
      }
      else {                                  //MAC addr1 and addr2 the same
        SsidStr3[0]='\0';                     //Set SSID to null
        Addr3Oui[0]='\0';                     //Set OUI to null
      }
      
      //
      // Produce the output
      //
      snprintf(beaconStr,BEACON_RECORD_SIZE,"%02x%02x%02x%02x%02x%02x,%02x%02x%02x%02x%02x%02x,%02x%02x%02x%02x%02x%02x,%s,%s,%s,%s,%s,%s,%d,%d\n",
          Addr1[0],Addr1[1],Addr1[2],Addr1[3],Addr1[4],Addr1[5],
          Addr2[0],Addr2[1],Addr2[2],Addr2[3],Addr2[4],Addr2[5],
          Addr3[0],Addr3[1],Addr3[2],Addr3[3],Addr3[4],Addr3[5],
          Addr1Oui,SsidStr1,Addr2Oui,SsidStr2,Addr3Oui,SsidStr3,
          ChannelNo,SequenceNo);
          UdpWrite(&beaconPort,SendIP,UDPBEACON_PORT,beaconStr);
          USBSerial.printf("%s",beaconStr);

    }
    leds[0] = 0x000000;
    FastLED.show();
  }
}