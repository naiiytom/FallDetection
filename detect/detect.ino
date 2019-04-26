#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>


const int MPU_addr=0x68; // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

//WiFi connection
String ssid = "GAMING HAUS";
String PASSWORD = "#p@ssw0rd";
String LINE_TOKEN = "9QhkSx5gEDNKUsbBw9eVziupjG5WSwyd99Qivd8izaj";

void connectWiFi(String id, String pass);
void LineNotify(String TOKEN, String message);

void setup() {
  Wire.begin();
  Serial.begin(115200);
  check_I2c(MPU_addr); // Check that there is an MPU
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  connectWiFi(ssid, PASSWORD);
  delay(2000);
  String message = "Hello, it's me sent from ESP8266!";
  LineNotify(LINE_TOKEN, message);
  delay(60000);
}


void loop() {
  /*Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  
  Wire.requestFrom(MPU_addr,14,true); // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY=Wire.read()<<8|Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  
  Serial.print("AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.print(AcZ);
  Serial.print(" | Tmp = "); Serial.print(Tmp);
  Serial.print(" | GyX = "); Serial.print(GyX);
  Serial.print(" | GyY = "); Serial.print(GyY);
  Serial.print(" | GyZ = "); Serial.println(GyZ);
*/
}
   
byte check_I2c(byte addr){
  byte error;
  Wire.beginTransmission(addr);
  error = Wire.endTransmission();
   
  if (error == 0){
  Serial.print(" Device Found at 0x");
  Serial.println(addr,HEX);
  } else {
  Serial.print(" No Device Found at 0x");
  Serial.println(addr,HEX);
  }
  return error;
}

void connectWiFi(String id, String pass){
  delay(100);
  WiFi.begin(id, pass);
  Serial.print(id);
  Serial.println(" . . . ");
  int i=0;
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(i++);
    Serial.println(" Wating to reconnect to WiFi. . .");
  }
  Serial.println("Connection established!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void LineNotify(String TOKEN, String message){
  WiFiClientSecure _client;

  if (_client.connect("notify-api.line.me", 443)) {
    Serial.println("[LineNotify] can't connect to notify-api.line.me");
    return;
  }
  int httpCode = 404;
  String body = "message=" + message;
  String request = "";
  request += "POST /api/notify HTTP/1.1\r\n";
  request += "Host: notify-api.line.me\r\n";
  request += "Authorization: Bearer " + String(TOKEN) + "\r\n";
  request += "Cache-Control: no-cache\r\n";   
  request += "User-Agent: ESP8266/ESP32\r\n";
  request += "Connection: close\r\n";   
  request += "Content-Type: application/x-www-form-urlencoded\r\n";
  request += "Content-Length: " + String(body.length()) + "\r\n\r\n" + body;

  
  _client.print(request);

  while( _client.connected() && !_client.available()) delay(10);
    if(_client.connected() && _client.available() ) {
      String resp = _client.readStringUntil('\n');
      httpCode = resp.substring(resp.indexOf(" ")+1, resp.indexOf(" ", resp.indexOf(" ")+1)).toInt();
    }
  _client.stop();    
}
