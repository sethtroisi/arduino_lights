#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <Adafruit_MCP9808.h>

#define PHOTORESISTOR_PIN   A0
#define TMP_36_PIN          A1
#define STATUS_LED_PIN      2
#define BS18X20_PIN         3

#define PER_MINUTE          6
 
// Create the MCP9808 temperature sensor object.
Adafruit_MCP9808 tempSensorMCP9808;

// Create TSL2561 (or 91?) light sensor object.
Adafruit_TSL2561_Unified lightSensorTSL2561(TSL2561_ADDR_FLOAT, 12345);



/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2561
*/
/**************************************************************************/
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
  if(!lightSensorTSL2561.begin())
  {
    shutdownAndBlinkHelp("Couldn't find LightSensor (TSL2561)");
  }

  configureLightSensorTSL2561();
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
    Serial.println("*C");


    float tempTMP36 = TMP36ReadTempC();    
    Serial.print("TMP36: ");
    Serial.print(tempTMP36);
    Serial.println("*C");
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
















/*





#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 3
#define TEMPERATURE_PRECISION 9

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature  (&oneWire);

int numberOfDevices; // Number of temperature devices found

DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();
  
  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();
  
  // locate devices on the bus
  Serial.print("Locating devices...");
  
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  
  // Loop through each device, print out address
  for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i))
	{
		Serial.print("Found device ");
		Serial.print(i, DEC);
		Serial.print(" with address: ");
		printAddress(tempDeviceAddress);
		Serial.println();
		
		Serial.print("Setting resolution to ");
		Serial.println(TEMPERATURE_PRECISION, DEC);
		
		// set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
		sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
		
		 Serial.print("Resolution actually set to: ");
		Serial.print(sensors.getResolution(tempDeviceAddress), DEC); 
		Serial.println();
	}else{
		Serial.print("Found ghost device at ");
		Serial.print(i, DEC);
		Serial.print(" but could not detect address. Check power and cabling");
	}
  }

}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  // method 1 - slower
  //Serial.print("Temp C: ");
  //Serial.print(sensors.getTempC(deviceAddress));
  //Serial.print(" Temp F: ");
  //Serial.print(sensors.getTempF(deviceAddress)); // Makes a second call to getTempC and then converts to Fahrenheit

  // method 2 - faster
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
}

void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  
  
  // Loop through each device, print out temperature data
  for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i))
	{
		// Output the device ID
		Serial.print("Temperature for device: ");
		Serial.println(i,DEC);
		
		// It responds almost immediately. Let's print out the data
		printTemperature(tempDeviceAddress); // Use a simple function to print out the data
	} 
	//else ghost device! Check your power requirements and cabling
	
  }
  delay(1000);
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

*/
