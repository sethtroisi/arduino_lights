#include <Adafruit_NeoPixel.h>
#include <string>

#define PIN_NUMBER 9

#define NUM_LIGHTS 50
#define BRIGHTNESS 40

#define DEBUG false

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

//  delay(1000);

  showHello(1);

  Serial.begin(9600);
//  Serial.println("Welcome to StrandClient");

  showHello(2);
  
  current_lights = A;
  next_lights = B;
}

char buffer[11] = {};
int buffI = 0
void loop() {
//  readSomeSerial();

  // echo program
  
  if (buffI == 10) {
    serial.println(buffer)
    buffI = 0;
  }

  buffer[buffI] = blockingSerialRead();
  buffI++;
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

void setAndShow(uint16_t i, uint32_t c) {
  strip.setPixelColor(i, c);
  strip.show();
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
    which[i] = strip.Color(0, 0, 0);
  }
}


void showLights(uint32_t *which) {
  assert(which == A || which == B, 11);

  for(uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, which[i]);
  }
  strip.show();
  delay(10);
}


bool readSomeSerial() {
  // Clear what ever is not current and
//  assert(current_lights != next_lights, 12);

  // TODO all sorts of zero length encoding, paletting, ... other compressions in the future!

  uint16_t iter = 0;
  while (true) {
    setAndShow(0, strip.Color(0,0,iter % 256));
    flushTillMagic();
    setAndShow(0, strip.Color(0,255,iter % 256));
    iter++;

    uint8_t index = blockingSerialRead();
    uint32_t serial_color = readColor();

    setAndShow(index + 1, serial_color);

  }
}

uint32_t readColor() {
 return strip.Color(blockingSerialRead(), blockingSerialRead(), blockingSerialRead());
}


uint32_t blockingSerialRead() { 
  while (Serial.available() < 1) {
    delayMicroseconds(1);
  }
  
  return Serial.read();
}

void flushTillMagic() {
  // Look for magic start of stream command (3, 5, 8, 13)
  while (true) {
    if (Serial.available() < 4) {
      continue;
    }

    int first = Serial.read();
    if (first != 3) {
      continue;
    }

    if (Serial.peek() != 5) {
      continue;
    }
    Serial.read();

    if (Serial.peek() != 8) {
      continue;
    }
    Serial.read();


    if (Serial.peek() != 13) {
      continue;
    }
    Serial.read();
    
    // Magic key found!
    return;
  }
}
