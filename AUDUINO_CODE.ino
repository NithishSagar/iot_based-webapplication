#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

#define RX 10
#define TX 11

#define SSID "V2023"
#define PASSWORD "nithish@123"

#define THINGSPEAK_API_KEY "YKF5WO4QNJUFAYTF"

#define SOIL_MOISTURE_PIN A1
#define PH_SENSOR_PIN A3

SoftwareSerial espSerial(RX, TX);
WiFiClient client;

int soilMoistureValue = 0;
float pHValue = 0.0;

void setup() {
  Serial.begin(9600);
  espSerial.begin(115200);
  connectToWiFi();
  ThingSpeak.begin(client);
}

void loop() {
  delay(2000);
  soilMoistureValue = readSoilMoisture();
  pHValue = readpH();

  Serial.print("Soil Moisture: ");
  Serial.println(soilMoistureValue);

  Serial.print("pH Value: ");
  Serial.println(pHValue);

  uploadDataToThingSpeak(soilMoistureValue, pHValue);

  delay(5000);
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi!");
}

int readSoilMoisture() {
  int sensorValue = analogRead(A0);
  int moisturePercentage = map(sensorValue, 0, 1023, 0, 100);
  return moisturePercentage;
}

float readpH() {
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1024.0);
  float pH = 3.5 * voltage + 0.0;
  return pH;
}

void uploadDataToThingSpeak(int soilMoisture, float pH) {
  ThingSpeak.setField(1, soilMoisture);
  ThingSpeak.setField(2, pH);
  int httpCode = ThingSpeak.writeFields(2202765, "6QOU5ODYA4CKR5VF");

  if (httpCode == 200) {
    Serial.println("Data uploaded to ThingSpeak successfully!");
  } else {
    Serial.println("Error uploading data to ThingSpeak. HTTP error code: " + String(httpCode));
  }
}
