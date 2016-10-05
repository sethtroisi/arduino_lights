#include <Adafruit_NeoPixel.h>

#define PIN_NUMBER 3
#define NUM_LIGHTS 50

#define BRIGHTNESS 255

#define DELAY_MS 85
#define FADE_INTERVALS 20


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


// Return colors that is a blend of a and b.
// a * (1 - percent) + b * (percent);
// 
uint32_t LinearColorFade(uint32_t color_a, uint32_t color_b, float percent) {
  uint32_t r_a = (uint8_t)(color_a >> 16);
  uint32_t r_b = (uint8_t)(color_b >> 16);
  int16_t r_diff = r_b - r_a;
  
  uint32_t g_a = (uint8_t)(color_a >> 8);
  uint32_t g_b = (uint8_t)(color_b >> 8);
  int16_t g_diff = g_b - g_a;

  uint32_t b_a = (uint8_t)(color_a);
  uint32_t b_b = (uint8_t)(color_b);
  int16_t b_diff = b_b - b_a;

  int16_t new_r = round(r_a + r_diff * percent);
  int16_t new_g = round(g_a + g_diff * percent);
  int16_t new_b = round(b_a + b_diff * percent);

  return strip.Color(new_r, new_g, new_b);
}

// Walk from 0 to (distance-1) then back to 0
// Period is 2 * (distance - 1)
// 0, 1, 2, 3, 2, 1
int  ThereAndBackAgain(int i, int distance) {
  // 3, 2, 1, 0, -1, -2
  int interval = (distance-1) + (-i % (2 * distance - 2));

  // 3, 2, 1, 0, 1, 2
  int absed = abs(interval);
  
  // 0, 1, 2, 3, 2, 1
  return (distance-1) - absed;
}

void setup() {
  // Silly calculation for BRIGHTNESS

  Serial.begin(9600); 
  
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  
  strip.clear();
  strip.show(); // Initialize all pixels to 'off'
}


int increment = -1;
void loop() {
  increment += 1;

//  Serial.print(increment);
//  Serial.print(" => ");
//  Serial.println(ThereAndBackAgain(increment, 5));

  // Should be odd
  int number_leds = 21;

  for (int i = 0; i < number_leds; i++) {
    // leds has period 5, 6, 7, 8, 9, 10, ..., 14, 15, 14, , ... 7, 6, 5
    int wave_period = 15 + 5 * ThereAndBackAgain(i, (number_leds + 1) / 2);

    float percent = ThereAndBackAgain(increment, wave_period) / (wave_period - 1.0);

    strip.setPixelColor(i, LinearColorFade(GREEN, PURPLE, percent));
  }
  strip.show();
  
  delay(DELAY_MS);
}
