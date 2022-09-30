
# Barometer to NMEA. . 

Simple wemos based barometer to send NMEA messages to wifi (UDP) or serial.
Creates a standard NMEA IIXDR message and calculates the checksum.



### Hardware

| Requirement           | Type |
| ----------------- | -----------------|
| ESP8266 | Wemos D1 mini | 
| Barometer | BMP280 |
| Buck Converter | Mini 560 |

### Sensor Wiring

| BMP280 | NodeMCU |
| ------ | ------ |
| VCC | 3.3v |
| GND | GND |
| SCL | D2 |
| SDA | D1|


### License

[MIT](https://choosealicense.com/licenses/mit/)

