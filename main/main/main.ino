#include "Wire.h"
#include "I2Cdev.h"
#include "TridentTD_LineNotify.h"
#include <ESP8266WiFi.h>
#include "MPU6050.h"

#define M_PI 3.14159265358979323846
#define LED_PIN 13

int16_t ax, ay, az;
int16_t gx, gy, gz;

int mean_ax,mean_ay,mean_az,mean_gx,mean_gy,mean_gz,state=0;
int ax_offset,ay_offset,az_offset,gx_offset,gy_offset,gz_offset;

MPU6050 acc;

String message = "Detect: Patient fell!";

#define LINE_TOKEN "9QhkSx5gEDNKUsbBw9eVziupjG5WSwyd99Qivd8izaj"
String ssid = "tomoto";
String pass = "1234567890";

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
  double svm, theta;
  acc.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  ax = ax/(1000);
  ay = ay/(1000);
  az = az/(1000);
  Serial.print("a/g:\t");
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
  Serial.print(az); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.println(gz);

  svm = sqrt(pow(ax, 2) + pow(ay, 2) + pow(az, 2));
  Serial.println(svm);
  theta = atan((sqrt(pow(ay, 2) + pow(az, 2))) / az) * (180 / M_PI);
  Serial.println(theta);
  delay(100);
  unsigned long startTime, currentTime;
  bool flag = false;
  if(svm >= 23.00  || theta < 1){
    Serial.println("Fall!");
    startTime = millis();
    flag = true;
    while(flag){
      Serial.println("Loop");
      acc.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      ax = ax/(1000);
      ay = ay/(1000);
      az = az/(1000);
      Serial.print("a/g:\t");
      Serial.print(ax); Serial.print("\t");
      Serial.print(ay); Serial.print("\t");
      Serial.print(az); Serial.print("\t");
      Serial.print(gx); Serial.print("\t");
      Serial.print(gy); Serial.print("\t");
      Serial.println(gz);
      svm = sqrt(pow(ax, 2) + pow(ay, 2)+ pow(az, 2));
      theta = atan((sqrt(pow(ay, 2) + pow(az, 2))) / az) * (180 / M_PI);
      bool sent = false;
      if(svm <= 18.0 && theta <1){
        Serial.println("Stable");
        currentTime = millis();
        if(currentTime - startTime >= 3000){
          if(!sent){
            Serial.println("Send Notify");
            LINE.notify(message);
            sent = true;
          }
        }
      }
      else{
        if(sent){
          Serial.println("Fall Duration");
          currentTime = millis();
          String msg = "Fall duration: " + String(int((currentTime - startTime)/1000)) + " seconds";
          LINE.notify(msg);
        }
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
