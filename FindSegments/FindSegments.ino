#include <Adafruit_NeoPixel.h>

#define PIN_NUMBER 3
#define NUM_LIGHTS 150
#define BRIGHTNESS 32


#define START_LED 10
#define END_LED 80

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN_NUMBER, NEO_GRB + NEO_KHZ800);

uint32_t RED   = strip.Color(255, 16, 16);
uint32_t GREEN = strip.Color(16, 255, 16);
uint32_t BLUE  = strip.Color(16, 16, 255);
uint32_t BLACK  = strip.Color(0, 0, 0);


void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

int iteration = 0;
void loop() {
  for (int led_index = START_LED; led_index < END_LED; led_index += 1) {
    strip.setPixelColor(led_index, GREEN);
    
    if (led_index % 5 == 0) {
      if (iteration % 2 == 0) {
        strip.setPixelColor(led_index, RED);
      } else {
        strip.setPixelColor(led_index, BLUE);
      }
    }
  }
  strip.show();
  delay (5000);

  iteration += 1;
}
