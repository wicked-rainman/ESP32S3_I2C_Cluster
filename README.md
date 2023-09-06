# ESP32S3_I2C_Cluster
<img src="https://github.com/wicked-rainman/ESP32S3_I2C_Cluster/images/Cluster.jpg" width="385px" align="center">
An ESP32S3 I2C cluster that captures WiFi beacon packets and outputs records that allow for a Network map to be drawn using the .dot process. The cluster consists four nodes: 

1. Performs a constant passive WiFi scan looking for netwoks.
2. Collects WiFi beacon packets.
3. Maps Mac addresses to vendor OUI 
4. (The master) polls each of the nodes for new data and outputs records accordingly
