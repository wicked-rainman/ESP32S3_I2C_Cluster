# ESP32S3_I2C_Cluster

An ESP32S3 I2C cluster that captures WiFi beacon packets and outputs records that allow for a Network map to be drawn using the .dot process. The cluster consists four nodes: 

* Performs a constant passive WiFi scan looking for netwoks.
* Collects WiFi beacon packets.
* Maps Mac addresses to vendor OUI 
* (The master) polls each of the nodes for new data and outputs records accordingly

<img src="https://github.com/wicked-rainman/ESP32S3_I2C_Cluster/blob/main/images/Cluster.jpg" width="400" align="center">

