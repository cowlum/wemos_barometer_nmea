
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <iomanip>

WiFiUDP UDP;

// Set WiFi credentials
#define WIFI_SSID "SSID"
#define WIFI_PASS "password"
#define UDP_PORT 5000  // May need to be changed to 5000
#define UDP_TARGET_IP "192.168.0.255"
unsigned long delayTime;
int restart_counter = 0;
float pressure_var = 0;
float pressure_adjust = -0.004;
//int soak_testing = 0;


// calibrate pressure
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP280 BMP;

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

  bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = BMP.begin(0x76);  
  //if (!status) {
  //  Serial.println("Could not find a valid BMP280 sensor, check wiring!");
  //  while (1);
  //}

  Serial.println("-- Default Test --");
  delayTime = 10000;

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


  //std::string tmp = std::to_string(soak_testing);
  //char const *num_char = tmp.c_str();

  //String soak_str = soak_testing;
  //int soak_len = soak_str.length() + 1;
  //char soak_array[soak_len]; 
  //soak_str.toCharArray(soak_array, soak_len);
  //UDP.beginPacket(UDP_TARGET_IP, UDP_PORT);
  //UDP.printf(num_char);
  //UDP.endPacket();
  //soak_testing++;
  
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
