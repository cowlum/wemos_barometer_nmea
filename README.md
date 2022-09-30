
# Barometer to NMEA. . 

Simple wemos based barometer to send NMEA messages to wifi (UDP) or serial.
Creates a standard NMEA IIXDR message and calculates the checksum.



### Hardware

| Hardware            | |
| ----------------- | -----------------|
| ESP8266 | Wemos D1 mini | 
| Barometer | BMP280 |
| buck converter | Mini 560 |

### Wiring

| BMP280 | NodeMCU |
| ------ | ------ |
| VCC | VCC or 3.3v |
| GND | GND |
| SCL | D2 |
| SDA | D1|


### License

[MIT](https://choosealicense.com/licenses/mit/)

