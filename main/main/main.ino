#include "Wire.h"
#include "I2Cdev.h"
#include "TridentTD_LineNotify.h"
#include <ESP8266WiFi.h>
#include "MPU6050.h"

#define M_PI 3.14159265358979323846
#define LED_PIN 13
bool blinkState = false;

int16_t ax, ay, az;
int16_t gx, gy, gz;

int mean_ax,mean_ay,mean_az,mean_gx,mean_gy,mean_gz,state=0;
int ax_offset,ay_offset,az_offset,gx_offset,gy_offset,gz_offset;

MPU6050 acc;

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
  delay(100);

  acc.setXAccelOffset(-3654);
  acc.setYAccelOffset(-2276);
  acc.setZAccelOffset(1239);
  acc.setXGyroOffset(27);
  acc.setYGyroOffset(29);
  acc.setZGyroOffset(-35);
  
  WiFiConnect(ssid, pass);
  LINE.setToken(LINE_TOKEN);
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  int svm, theta;
  acc.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  Serial.print("a/g:\t");
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
  Serial.print(az); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.println(gz);

  svm = sqrt(pow(ax, 2) + pow(ay, 2) + pow(az, 2));
  //theta = atan((sqrt(pow(ay, 2) + pow(az, 2))) / az) * (180 / M_PI);
  unsigned long start, current;
  bool flag = false;
  if(svm > 2.5){
    start = millis();
    flage = true;
    while(flag){
      acc.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      svm = sqrt(pow(ax, 2) + pow(ay, 2) + pow(az, 2));
      //theta = atan((sqrt(pow(ay, 2) + pow(az, 2))) / az) * (180 / M_PI);
      if(svm < 1.0){
        current = millis();
        if(current - start >= 3000){
          LINE.notify(message);
        }
      }
      else{
        flag = false;
      }
      delay(10);
    }
  }
  delay(10);
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
