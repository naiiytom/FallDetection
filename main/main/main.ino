#include "Wire.h"
#include "I2Cdev.h"
#include "TridentTD_LineNotify.h"
#include <ESP8266WiFi.h>
#include "MPU6050.h"

#define LED_PIN 13
bool blinkState = false;

int16_t ax, ay, az;
int16_t gx, gy, gz;

MPU6050 acc(0x68);

String message = "แจ้งเตือนมีคนหกล้ม";

#define LINE_TOKEN "9QhkSx5gEDNKUsbBw9eVziupjG5WSwyd99Qivd8izaj"
String ssid = "GAMING HAUS";
String pass = "#p@ssw0rd";

void WiFiConnect(String id, String pasword);

void setup()
{
  delay(100);
  Wire.begin();
  Serial.begin(115200);
  delay(100);
  Serial.println("Init MPU6050");
  acc.initialize();
  
  Serial.println("Testing device connections...");
  Serial.println(acc.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  delay(10000);
  WiFiConnect(ssid, pass);
  LINE.setToken(LINE_TOKEN);
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  acc.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  Serial.print("a/g:\t");
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
  Serial.print(az); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.println(gz);
  
  //if((pitch>35) || (pitch<-35) || (roll>35) || (roll<-35)){
  //  LINE.notify(message);
  //}
}

void WiFiConnect(String id, String password)
{
  delay(100);
  Serial.print("Connecting => ");
  Serial.println(id);

  WiFi.begin(id, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(". ");
  }
  Serial.println();
  Serial.print("WiFi Connected\nIP address: ");
  Serial.println(WiFi.localIP());
}
