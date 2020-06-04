#include <Wire.h>
#include <ESP8266WiFi.h>

const uint8_t MPU_addr=0x68;
int16_t AcX, AcY, AcZ, TmP,GyX,GyY,GyZ;
float AcX_calc, AcY_calc, AcZ_calc;
uint32_t lastTime;

const char* ssid = "XXXXXXXXXXX";      // Your Mobile Hotspot Name
const char* password = "XXXXXXXXXXX";  // Your Mobile Hotspot Password


WiFiClient client; // Initialize the client library

String MakerIFTTT_Key ;
;String MakerIFTTT_Event;
char *append_str(char *here, String s) {  int i=0; while (*here++ = s[i]){i++;};return here-1;}
char *append_ul(char *here, unsigned long u) { char buf[20]; return append_str(here, ultoa(u, buf, 10));}
char post_rqst[256];char *p;char *content_length_here;char *json_start;int compi;
 

void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.println("Fall 2 Alert System Initiated");
  delay(3000);
  Wire.begin(4,5);
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  WiFi.begin(ssid,password);
  Serial.print("Connecting to WiFi - ");
  Serial.println(ssid);
  while (WiFi.status()!=WL_CONNECTED){
    delay(500);
  }
  Serial.println();
  Serial.print("Connected:");
  Serial.println(WiFi.localIP());
   delay(500);


}



void loop() {

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


  if(abs(AcX_calc)> 22000 || abs(AcY)> 22000|| abs(AcZ) > 27000)
  
  {
    
    if (client.connect("maker.ifttt.com",80)) {  
    MakerIFTTT_Key ="dR6FcZfAy9-QBHEns6ok-XXXXXXXXXXXX"; // IFTTT key (Go to Webhooks documentation)
    MakerIFTTT_Event ="fall_alert";  // IFTTT event as mentioned on Applet
    p = post_rqst;
    p = append_str(p, "POST /trigger/");
    p = append_str(p, MakerIFTTT_Event);
    p = append_str(p, "/with/key/");
    p = append_str(p, MakerIFTTT_Key);
    p = append_str(p, " HTTP/1.1\r\n");
    p = append_str(p, "Host: maker.ifttt.com\r\n");
    p = append_str(p, "Content-Type: application/json\r\n");
    p = append_str(p, "Content-Length: ");
    content_length_here = p;
    p = append_str(p, "NN\r\n");
    p = append_str(p, "\r\n");
    json_start = p;
    compi= strlen(json_start);
    content_length_here[0] = '0' + (compi/10);
    content_length_here[1] = '0' + (compi%10);
    client.print(post_rqst);
    }
    Serial.println("Fall detected!"); 
    delay(5000);
  }
  
 
  Serial.print("AcX: "); Serial.print(AcX); Serial.print("g | AcY: "); Serial.print(AcY); Serial.print("g | AcZ: "); Serial.print(AcZ);
  Serial.println("g");
  delay(333);

}
