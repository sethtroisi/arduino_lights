#include <Adafruit_NeoPixel.h>

#define PIN_NUMBER 9

#define NUM_LIGHTS 50
#define BRIGHTNESS 80

#define DEBUG true

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN_NUMBER, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.


uint32_t A[NUM_LIGHTS];
uint32_t B[NUM_LIGHTS];
uint32_t *current_lights, *next_lights;

void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  delay(1000);

  showHello(1);

  Serial.begin(57600);
  Serial.println("Welcome to StrandClient");

  // "Flush any serial data"
  while (Serial.peek() > 0) { Serial.read(); };

  showHello(2);
  
  current_lights = A;
  next_lights = B;
}

void loop() {
  readSomeLights();
}


void showHello(uint16_t hello_status) {
  uint32_t tiny_red =  strip.Color(10, 4, 4);
  uint32_t red = strip.Color(50, 0, 0);
  uint32_t green =  strip.Color(0, 50, 0);
  uint32_t blue =  strip.Color(0, 0, 50);
  
  // Mark everything black
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, (i % 5 == 0) ? red : tiny_red);
  }

  for(uint16_t i = 1; i <= 4; i++) {
    strip.setPixelColor(i, (hello_status > 0) ? green : red);
  }


  for (uint16_t i = 1; i <= hello_status; i++) {
    strip.setPixelColor(5 * i + 1, blue);
    strip.setPixelColor(5 * i + 2, blue);
    strip.setPixelColor(5 * i + 3, blue);
    strip.setPixelColor(5 * i + 4, blue);
  }

  strip.show();
  delay(5000);
}


void assert(bool res, uint16_t label) {
  if (!res) {
    Serial.print("Assert failed label: "); 
    Serial.println(label); 
    
    while (1) {
      delay(100);
    }    
  }
}


void setLightsBlack(uint32_t *which) {
  assert(which == A || which == B, 10);
  
  for (int i = 0; i < NUM_LIGHTS; i++) {
    which[i] = 0;
  }
}


bool readSomeLights() {
  // Clear what ever is not current and
  assert(current_lights != next_lights, 11);
  // TODO all sorts of zero length encoding, paletting, ... other compressions in the future!
  
  // Look for magic start of stream command (112, 35, 8, X)
  while (Serial.available() > 5) {
    Serial.println("Starting readSomeLights, available: ");
    Serial.println(Serial.available());

    int first = Serial.read();
    if (first != 112) {
      continue;
    }

    if (Serial.peek() != 35) {
      continue;
    }
    Serial.read();

    if (Serial.peek() != 8) {
      continue;
    }
    Serial.read();
  }

  // Data should be alligned
  uint32_t data_size;
  if (!readUnsignedInt(&data_size)) {
    return false;
  }

  if ((data_size < 0) || (data_size % 3 != 0) || (data_size <= 3 * NUM_LIGHTS)) {
    return false;
  }
  
  setLightsBlack(next_lights);
  
  for (int i = 0; i <= data_size / 3; i++) {
    next_lights[i] = readColor();
  }
  
  uint32_t *temp = current_lights;
  current_lights = next_lights;
  next_lights = temp;
  
  if (DEBUG) {
    Serial.print("Data_size: ");
    Serial.println(data_size);
    
    for (int i = 0; i <= data_size / 3; i++) {
      Serial.print("RGB Color(");
      Serial.print(i);
      Serial.print("): ");
      Serial.print(current_lights[i]);
    }
  }
}


bool readUnsignedInt(uint32_t *out_value) {
  if (Serial.available() <= 3) {
    return false;
  }

  *out_value = (Serial.read() << 24) \
      | (Serial.read() << 16) \
      | (Serial.read() << 8) \
      | Serial.read();

  return true;
}


uint32_t readColor() {
 return strip.Color(Serial.read(), Serial.read(), Serial.read());
}


/*
void loop() {
  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(255, 0, 0), 15); // Red
  colorWipe(strip.Color(0, 255, 0), 15); // Green
  colorWipe(strip.Color(0, 0, 255), 15); // Blue
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
*/
