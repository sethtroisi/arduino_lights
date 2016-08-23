#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <Adafruit_MCP9808.h>


#define PHOTORESISTOR_PIN   A0
#define TMP_36_PIN          A1
#define STATUS_LED_PIN      2

#define ONE_WIRE_BUS        3
#define ONE_WIRE_SENSORS    1
#define DS18B20_PRECISION   9

// Approximate number of data points per minute.
#define PER_MINUTE          3
 

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature  oneWireTempSensors(&oneWire);
DeviceAddress oneWireAddress[ONE_WIRE_SENSORS];

Adafruit_MCP9808 tempSensorMCP9808;
Adafruit_TSL2561_Unified lightSensorTSL2561(TSL2561_ADDR_FLOAT, 12345);


void configureLightSensorTSL2561(void)
{
  /* You can also manually set the gain or enable auto-gain support */
  // .setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // .setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  lightSensorTSL2561.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  //.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  //.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  lightSensorTSL2561.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */
}


bool configureDS18B20(void) {
  int numFound = oneWireTempSensors.getDeviceCount();
  if (ONE_WIRE_SENSORS != numFound) {
    Serial.print("ERROR: Found ");
    Serial.print(numFound);
    Serial.println(" one wire devices");
    return false;
  }

  for (int i = 0; i < ONE_WIRE_SENSORS; i++) {
    if (oneWireTempSensors.getAddress(oneWireAddress[i], i)) {
      // set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
      oneWireTempSensors.setResolution(oneWireAddress[i], DS18B20_PRECISION);
      int resolution = oneWireTempSensors.getResolution(oneWireAddress[i]);
      if (DS18B20_PRECISION != resolution) {
        Serial.print("ERROR: Resolution set to ");
        Serial.println(resolution);
        return false;
      }
    } else {
      Serial.print("Error: Found ghost device at ");
      Serial.print(i);
      Serial.print(" but could not detect address. Check power and cabling");
      return false;
    }
  }
  return true;
}



int getMilliVoltsAnalogPin(int pin) {
  int analogValue = analogRead(pin);
  return map(analogValue, 0, 1023, 0, 5000);
}

float TMP36ReadTempC() {
  int milliVolts = getMilliVoltsAnalogPin(TMP_36_PIN);  
  return (milliVolts - 500) / 10.0;
}


void setStatusLED(bool on) {
  digitalWrite(STATUS_LED_PIN, on);
}


void shutdownAndBlinkHelp(const char* error) {
  Serial.println(error);

  // Shutdown sensors and stuff
  tempSensorMCP9808.shutdown_wake(1);

  while (1) {
    setStatusLED(true);
    delay(1000);
    setStatusLED(false);
    delay(1000);    
  }  
}

// /*
void setup() {
  pinMode(PHOTORESISTOR_PIN, INPUT);
  pinMode(TMP_36_PIN, INPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);

  for (int i = 0; i < 8; i++) {
    setStatusLED(true);
    delay(50);
    setStatusLED(false);
    delay(200);
  }

  Serial.begin(9600);
  Serial.println("Sensor Pack! Hello World!");

  if (!tempSensorMCP9808.begin()) {
    shutdownAndBlinkHelp("Couldn't find TempSensor (MCP9808)!");
  }

  if (lightSensorTSL2561.begin()) {
    configureLightSensorTSL2561();
  } else  {
    shutdownAndBlinkHelp("Couldn't find LightSensor (TSL2561)");
  }

  oneWireTempSensors.begin();
  if (!configureDS18B20()) {
    shutdownAndBlinkHelp("Couldn't configure OneWireSensors (DS18b20)");
  }
}


void loop() {
  // Tempurate Sensors
  {
    float tempMCP9808 = tempSensorMCP9808.readTempC();
    // Sensor will read the same value continuesly unless shutdown and restart.
    tempSensorMCP9808.shutdown_wake(1);
    tempSensorMCP9808.shutdown_wake(0);   
    Serial.print("MCP9808: ");
    Serial.print(tempMCP9808);
    Serial.println(" *C");


    float tempTMP36 = TMP36ReadTempC();    
    Serial.print("TMP36: ");
    Serial.print(tempTMP36);
    Serial.println(" *C");


    // OneWire DS18B20 sensors.
    oneWireTempSensors.requestTemperatures(); // Send the command to get temperatures
    for(int i = 0; i < ONE_WIRE_SENSORS; i++) {
      float tempDS18B20 = oneWireTempSensors.getTempC(oneWireAddress[i]);
      Serial.print("DS18B20(");
      Serial.print(i);
      Serial.print("): ");
      Serial.print(tempDS18B20);
      Serial.println(" *C");
    }
  }

  // Light Sensors
  {
    sensors_event_t event;
    lightSensorTSL2561.getEvent(&event);
    // If event.light = 0 lux the sensor is probably saturated
    //   and no reliable data could be generated!
    if (event.light) {
      Serial.print("TSL2561: ");
      Serial.print(event.light);
      Serial.println(" lux");
    }


    int lightPhotoresistor = getMilliVoltsAnalogPin(PHOTORESISTOR_PIN);
    Serial.print("Photoresistor: ");
    Serial.print(lightPhotoresistor);
    Serial.println(" mv");
  }

  Serial.println();

  int delayMS = 60000 / PER_MINUTE;
  delay(delayMS);
}


