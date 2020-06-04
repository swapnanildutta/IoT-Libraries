// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor


#include <DHT.h>
#define DHTPIN D5    // Digital pin connected to the DHT 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
uint8_t temperature, humidity;

void setup() {
  Serial.begin(115200);
dht.begin();
delay(10);
Serial.println();
Serial.println();

}

void loop() {

// Wait a few seconds between measurements.
delay(500);  
  
// Reading temperature or humidity takes about 250 milliseconds!
// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
temperature = dht.readTemperature();
humidity = dht.readHumidity();
  
Serial.print("Temperature Value is: ");
Serial.print(temperature);
Serial.print("°C");
Serial.print("  Humidity Value is: ");
Serial.print(humidity);
Serial.println("%");
}
