#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

#define UART_TX 16
#define UART_RX 17

SoftwareSerial ard_Serial(UART_RX, UART_TX);

WiFiClient client;

const char *ssid = "V2030";
const char *pass = "nithish@123";



#define THINGSPEAK_API_KEY "YKF5WO4QNJUFAYTF"

int httpCode = 0;
int soilMoistureValue = -1;
float pHValue = -1.0f;

void setup() {
  // put your setup code here, to run once:
  ard_Serial.begin(115200);
  Serial.begin(115200);
  while(!Serial){
    //DO NOTHING
  }
  connectToWiFi();
  ThingSpeak.begin(client);
}

void loop() {
  // put your main code here, to run repeatedly:
  soilMoistureValue = (int)ard_Serial.read();
  pHValue = (float)ard_Serial.read();
  if(soilMoistureValue >= 0 && pHValue >= 0){
    uploadDataToThingSpeak(soilMoistureValue, pHValue);
  }
  soilMoistureValue = -1;
  pHValue = -1.0f;
}

int connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  return 0;
  Serial.println("Connected to WiFi!");
}

void uploadDataToThingSpeak(int soilMoisture, float pH) {
  ThingSpeak.setField(1, soilMoisture);
  ThingSpeak.setField(2, pH);
  httpCode = ThingSpeak.writeFields(2202765, "6QOU5ODYA4CKR5VF");

  if (httpCode == 200) {
    Serial.println("Data uploaded to ThingSpeak successfully!");
  } else {
    Serial.println("Error uploading data to ThingSpeak. HTTP error code: " + String(httpCode));
  }
}
