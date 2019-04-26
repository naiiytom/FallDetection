#include <TridentTD_LineNotify.h>
#include <ESP8266WiFi.h>
#include <MPU6050.h>

#define LINE_TOKEN "9QhkSx5gEDNKUsbBw9eVziupjG5WSwyd99Qivd8izaj"
String ssid = "GAMING HAUS";
String pass = "#p@ssw0rd";

void WiFiConnect(String id, String pasword);

void setup() {
  delay(100);
  Serial.begin(115200);
  WiFiConnect(ssid, pass);
  delay(1000);
  LINE.setToken(LINE_TOKEN);
}

void loop() {
  String message = "แจ้งเตือนมีคนหกล้ม";
  LINE.notify(message);
  Serial.println("...");
  delay(20000);
}

void WiFiConnect(String id, String password) {
  delay(100);
  Serial.print("Connecting => ");
  Serial.println(id);

  WiFi.begin(id, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(". ");
  }
  Serial.println();
  Serial.print("WiFi Connected\nIP address: ");
  Serial.println(WiFi.localIP());
}
