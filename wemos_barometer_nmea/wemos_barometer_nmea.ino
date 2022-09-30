
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <iomanip>

WiFiUDP UDP;
Adafruit_BMP280 BMP;

// Set WiFi credentials
#define WIFI_SSID "LittleFish"
#define WIFI_PASS ""
#define UDP_PORT 5000  // May need to be changed to 5000
#define UDP_TARGET_IP "192.168.0.255"
unsigned long delayTime = 10000;
int restart_counter = 0;
float pressure_var = 0;
// calibrate pressure
#define SEALEVELPRESSURE_HPA (1013.25)
float pressure_adjust = 0.000;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.mode(WIFI_STA);
  Serial.begin(38400);
  Serial.println(F("BMP280 test"));
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.setOutputPower(0);  // 0  lowest, 20.5 higest

  // Connecting to WiFi...
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  // Loop continuously while WiFi is not connected
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
    restart_counter++;
    if (restart_counter > 120){
      ESP.restart();
    }
  }

  BMP.begin(0x76);                    
    /* Default settings from datasheet. */
  BMP.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_1000); /* Standby time. */          

  Serial.println("-- Default Test --");
  Serial.println();
}


void loop() { 
  check_connectivity();
  printValues();
  udp_send();
  delay(delayTime);
}
  

void check_connectivity(){
 if (WiFi.status() != WL_CONNECTED){
    delay(1500000);
    ESP.restart();
  }
}

void printValues() {
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("Pressure = ");
  Serial.print(BMP.readPressure() / 100.0F);
  Serial.println(" hPa");
  pressure_var = BMP.readPressure() / 100000.0F + pressure_adjust;
  Serial.println();
  delay(50);
  digitalWrite(LED_BUILTIN, HIGH); 
}

void udp_send() {
  Serial.print("the pressure_var value = ");
  Serial.println(pressure_var, 5);
  String nmea_rpm_str = "$IIXDR,P," + String(pressure_var, 5) + ",B,BARO";
  int nmea_len = nmea_rpm_str.length() + 1;
  char nmea_array[nmea_len]; 
  nmea_rpm_str.toCharArray(nmea_array, nmea_len);
  Serial.printf("%s%s%X\n", nmea_rpm_str.c_str(),"*", nmea0183_checksum(nmea_array));
  
  Serial.println(pressure_var);
  UDP.beginPacket(UDP_TARGET_IP, UDP_PORT);
  UDP.printf("%s%s%X\n", nmea_rpm_str.c_str(),"*", nmea0183_checksum(nmea_array));
  UDP.endPacket();  
  
}

int nmea0183_checksum(char *nmea_array){  // https://forum.arduino.cc/t/nmea0183-checksum/559531/2
    int crc = 0;
    int i;
    // ignore the first $ sign,  no checksum in sentence
    for (i = 1; i < strlen(nmea_array); i ++) { 
        crc ^= nmea_array[i];
    }
    return crc;
}
