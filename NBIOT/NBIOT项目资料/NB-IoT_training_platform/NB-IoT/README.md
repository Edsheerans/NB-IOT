#nb-node
NB-IoT节点程序
## 1. Introduction ##

This project implement NB-IOT uplink.it get position data from gps, humidity and temperature from SHT2x every dutycycle. Then send these data to specified ip:port.

## 2. Hardware Platform ##

* MCU: stm32l051c8
* nb-module: lierda NB05-01
* Frequency: 850Mhz
* Sensor: GPS(SKG09A), SHT2x.

## 3. Packet Parse ##

* Application palyload length: 16
* Packet Define: Byte 0 ~ Byte 3:node id Byte 4 ~ Byte 6:latitude; Byte 7 ~ Byte 9:longitude; Byte 10,11:altitude; Byte 12,13:temperature; Byte 14,15:humidity.
* Example:

```
#!c
/* Node id*/
int node_id = (AppData[0] << 24) | (AppData[1] << 16) | (AppData[2] << 8) | AppData[3]; 
/* Latitude Parse */
double Latitude = ((AppData[4] << 16) | (AppData[5] << 8) | (AppData[6]));//Get raw data
if( Latitude >= 0 ) // North
    Latitude = (Latitude * 90) / 8388607;
else
    Latitude = (Latitude * 90) / 8388608;

/* Longitude Parse */
double Longitude = ((AppData[7] << 16) | (AppData[8] << 8) | (AppData[9]));//Get raw data
if( Longitude >= 0 ) // North
    Longitude = (Longitude * 180) / 8388607;
else
    Longitude = (Longitude * 180) / 8388608;

/* Altitude Parse */
int16_t Altitude = ((AppData[10] << 8) | (AppData[11]));

/* Temperature Parse */
double Temperature = ((AppData[12] << 8) | (AppData[13]));//Get raw data
Temperature = (Temperature * 175.72) / 65536 - 46.85;

/* Humidity Parse */
double Humidity = ((AppData[14] << 8) | (AppData[15]));//Get raw data
Humidity = (Humidity * 125) / 65536 - 6;
```

## 4. Changelog ##
**first version: 2017-07-11**

1.Implements NB-IOT uplink and use udp to send packet. 

2.Add some sensor data to user's packet and send to specified ip:port use udp.