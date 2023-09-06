# ESP32S3_I2C_Cluster
An ESP32S3 I2C cluster that captures WiFi beacon packets and outputs records that allow for a Network map to be drawn.
The cluster has four nodes. One performs a constant passive WiFi scan looking for netwoks. The secon node collects WiFi
beacon packets. The third maps Mac addresses to OUI and the fourth (the master) polls each of the nodes for new data.
