#include "udp_utils.h"
//--------------------------------------------------------------------
void UdpWrite(WiFiUDP *Mystream, IPAddress Sendip, int port,char *msg) {
    Mystream->beginPacket(Sendip, port);
    Mystream->print(msg);
    Mystream->endPacket();
}