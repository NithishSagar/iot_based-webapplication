#include <SoftwareSerial.h>


#define RX 5
#define TX 6



#define SOIL_MOISTURE_PIN A1
#define PH_SENSOR_PIN A3

SoftwareSerial espSerial(RX, TX);


int soilMoistureValue = 0;
float pHValue = 0.0;

void setup() {
  Serial.begin(9600);
  espSerial.begin(115200);
  
}

void loop() {
  delay(2000);
  soilMoistureValue = readSoilMoisture();
  pHValue = readpH();

  Serial.print("Soil Moisture: ");
  Serial.println(soilMoistureValue);
  espSerial.write(soilMoistureValue);
  Serial.print("pH Value: ");
  Serial.println(pHValue);
  espSerial.write(pHValue);
  

  delay(5000);
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


