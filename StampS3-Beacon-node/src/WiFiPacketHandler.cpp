#include "WiFiPacketHandler.h"
void WiFiPacketHandler(void* buff, wifi_promiscuous_pkt_type_t type) {
    if (type != WIFI_PKT_MGMT) return;
    const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buff;
    const wifi_ieee80211_packet_t *ipkt = (wifi_ieee80211_packet_t *)ppkt->payload;
    const wifi_ieee80211_mac_hdr_t *hdr = &ipkt->hdr;

    if(xSemaphoreTake(xShmem,portMAX_DELAY) == pdTRUE) {
        memcpy(BeaconStore[RingBuffPos]+0,hdr->addr1,6);
        memcpy(BeaconStore[RingBuffPos]+6,hdr->addr2,6);
        memcpy(BeaconStore[RingBuffPos]+12,hdr->addr3,6);
        BeaconStore[RingBuffPos][18] = channelSequence[channel];
        BeaconStore[RingBuffPos][19] = RingBuffPos;
        RingBuffPos++; //uint8_t rolls with the flow
        xSemaphoreGive(xShmem);
    }
}
