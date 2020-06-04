#include <Wire.h>
#include <FirebaseArduino.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>
#include <Firebase.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const uint8_t MPU_addr=0x68;
int16_t AcX, AcY, AcZ, TmP,GyX,GyY,GyZ;
float AcX_calc, AcY_calc, AcZ_calc;
uint32_t lastTime;

#define FIREBASE_HOST "fall2alert.firebaseio.com" // Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "3zFpv44luOgYJ8HupGAeBVvCCMtZhml4Escm0taF" // Your Firebase Database Secret goes here

#define WIFI_SSID "RASPI-02" //your WiFi SSID for which yout NodeMCU connects
#define WIFI_PASSWORD "RASPBERRY"//Password of your wifi network 


#define LED_PIN 14
#define BUTTON_PIN 12

void setup() {
  Serial.begin(115200);
  Serial.println("Fall 2 Alert System Initiated");
  Wire.begin(4,5);
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  Serial.print("Connecting to WiFi - ");
  Serial.println(WIFI_SSID);
  while (WiFi.status()!=WL_CONNECTED){
    delay(500);
  }
  Serial.println();
  Serial.print("Connected:");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  delay(500);


}

void firebasereconnect() {
  Serial.println("Trying to reconnect");
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Serial.println();
  }

void loop() {

  if (Firebase.failed()) {
      Serial.print("Failed to connect");
      Serial.println(Firebase.error());
      Serial.println();
      Serial.println();
      firebasereconnect();
      return;
  }

  
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, (uint8_t)14);
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)   
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)


  if(abs(AcX_calc)> 22000 || abs(AcY)> 22000|| abs(AcZ) > 27000){
    Serial.println("Fall detected");
    int distress_val = 1;
    lastTime = millis();
    if(millis()-lastTime<5000){
      distress_val = digitalRead(BUTTON_PIN);
      Serial.println(distress_val);
        Firebase.pushInt("Distress Code", distress_val);
      Serial.println("Alarm sent!");
      delay(10); //Debounce
    }
    delay(50);
  }
  
  if(millis()>(pow(2,32)-5000)){
    ESP.reset();
    }

  Serial.print("AcX: "); Serial.print(AcX); Serial.print("g | AcY: "); Serial.print(AcY); Serial.print("g | AcZ: "); Serial.print(AcZ);
  Serial.println("g");
  Firebase.setFloat("X-axis: ", float(AcX));
  Firebase.setFloat("Y-axix: ", float(AcY));
  Firebase.setFloat("Z-axis: ", float(AcZ));

  delay(333);

}
