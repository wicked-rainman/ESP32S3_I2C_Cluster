struct Store {
        uint8_t MacAddress[6];
        char Ssid[MAX_SSID_LENGTH];
        char Ssid_Assoc[MAX_SSID_LENGTH];
        int32_t Rssi;
        char rolling;
};