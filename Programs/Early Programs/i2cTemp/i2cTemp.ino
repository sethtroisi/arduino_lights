/**************************************************************************/
/*!
This is a demo for the Adafruit MCP9808 breakout
----> http://www.adafruit.com/products/1782
Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!
*/
/**************************************************************************/

#include <Wire.h>
#include "Adafruit_MCP9808.h"

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();
int sensorPin = A0;    // select the input pin for the potentiometer

void setup() {
  Serial.begin(9600);
  Serial.println("MCP9808 demo hello");
  
  // Make sure the sensor is found, you can also pass in a different i2c
  // address with tempsensor.begin(0x19) for example
  if (!tempsensor.begin()) {
    Serial.println("Couldn't find MCP9808!");
    while (1);
  }

  tempsensor.shutdown_wake(0);   // Don't remove this line! required before reading temp
  delay(1000);
}

void loop() {
  float sensorValue = analogRead(sensorPin);
  Serial.print("Light Value: ");
  Serial.println(sensorValue);


  // Read and print out the temperature, then convert to *F
  float c = tempsensor.readTempC();
  Serial.print("Temp: ");
  Serial.print(c);
  Serial.println("*C");

  Serial.println();
  delay(750);

//  tempsensor.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere
//  delay(1500);
}
