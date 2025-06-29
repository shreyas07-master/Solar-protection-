#include <Servo.h>
#include <DHT.h>

// Pins
#define rainSensor1Pin A0  // analog pin A0
#define rainSensor2Pin A1  // analog pin A1
#define ldrPin A2          // analog pin A2
#define dhtPin 7           // digital pin D7

#define DHTTYPE DHT11  // Use DHT11 or DHT22(Humidity sensor) based on your sensor


// make the adjustment based on the weather conditions 
// Threshold avalues (tune according to testing)
const int rainThreshold = 500;
const int lightThreshold = 800;
const float humidityThreshold = 60.0;

DHT dht(dhtPin, DHTTYPE);

// Servo objects
Servo servo1;
Servo servo2;

// Track tarpaulin status
bool isClosed = false;

void setup() {
  Serial.begin(9600);
  dht.begin();

  servo1.attach(9);       // servo connection to arduino pin D9
  servo2.attach(10);      // servo connection to arduino pin D10

  // Start with tarpaulin open
  openTarpaulin();
}

void loop() {
  int rain1 = analogRead(rainSensor1Pin);
  int rain2 = analogRead(rainSensor2Pin);
  int ldrValue = analogRead(ldrPin);
  float humidity = dht.readHumidity();

  Serial.print("Rain1: "); Serial.print(rain1);
  Serial.print(" | Rain2: "); Serial.print(rain2);
  Serial.print(" | LDR: "); Serial.print(ldrValue);
  Serial.print(" | Humidity: "); Serial.println(humidity);

  bool rainDetected = (rain1 < rainThreshold || rain2 < rainThreshold);
  bool highLight = (ldrValue > lightThreshold);
  bool highHumidity = (!isnan(humidity) && humidity > humidityThreshold);

  if (rainDetected) {
    Serial.println("Rain detected! Closing tarpaulin.");  // closing the tarpaulin when it detects rain
    closeTarpaulin();
  } else if (highLight || highHumidity) {
    Serial.println("High sunlight or humidity, opening tarpaulin.");  //opening tarpaulin after rain stop
    openTarpaulin();
  } else {
    Serial.println("Conditions normal, maintaining current tarpaulin state.");
  }

  delay(3000);  // Adjust delay as your need
}

void closeTarpaulin() {
  if (!isClosed) {
    // Rotate servo to close position
    servo1.write(0);   // Adjust angles as per mechanism
    servo2.write(0);
    isClosed = true;
    Serial.println("Tarpaulin Closed.");
  }
}

void openTarpaulin() {
  if (isClosed) {
    // Rotate servo to open position
    servo1.write(90);  // Adjust angles as per mechanism
    servo2.write(90);
    isClosed = false;
    Serial.println("Tarpaulin Opened.");
  }
}
