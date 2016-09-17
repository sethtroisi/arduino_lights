#include <Adafruit_NeoPixel.h>

#define PIN_NUMBER 3
#define NUM_LIGHTS 150

#define BRIGHTNESS 255
#define MAX_AMPS 9.0

#define DELAY_MS 35


// Useful to count "arrays"
// uint32_t colors = {WHITE, BLACK, BLUE}
// num_elements(colors) == 3
#define num_elements(x)  (sizeof(x) / sizeof((x)[0]))


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN_NUMBER, NEO_GRB + NEO_KHZ800);

// Definition of some Colors which are used multiple times
uint32_t RED    = strip.Color(255, 0, 0);
uint32_t GREEN  = strip.Color(0, 255, 0);
uint32_t BLUE   = strip.Color(0, 0, 255);
uint32_t PURPLE = strip.Color(64, 0, 176);

uint32_t BLACK  = strip.Color(0, 0, 0);
uint32_t WHITE  = strip.Color(255, 255, 255);

uint32_t fun_colors[] = {PURPLE, WHITE, GREEN, PURPLE, BLACK, WHITE};



float DetermineMaxBrightness() {
  float draw_per_brightness = 0.04 * 3 *  NUM_LIGHTS / 255.0;

  Serial.println(draw_per_brightness);
  
  // Respect max BRIGHTNESS from #defines
  return max(1, min(BRIGHTNESS, MAX_AMPS / draw_per_brightness));
}


void setup() {
  // Silly calculation for BRIGHTNESS

  Serial.begin(9600);
  Serial.println(DetermineMaxBrightness());
  
  for (int i = 0; i < 10000; i++) {
    delay(1000);
    Serial.println(i);
  }
  
  
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  
  strip.clear();
  strip.show(); // Initialize all pixels to 'off'
}


void loop() {
  delay(DELAY_MS);
}
