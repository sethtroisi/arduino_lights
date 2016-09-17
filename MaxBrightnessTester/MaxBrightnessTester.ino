#include <Adafruit_NeoPixel.h>

#define PIN_NUMBER 3
#define NUM_LIGHTS 150

#define BRIGHTNESS 255
#define MAX_AMPS 2.5

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



int DetermineMaxBrightness() {
  float draw_per_brightness = 0.015 * 3 *  NUM_LIGHTS / 255.0;

  // Respect max BRIGHTNESS from #define
  int max_brightness = max(1, min(BRIGHTNESS, MAX_AMPS / draw_per_brightness));

  Serial.print("Amps/brightness: ");
  Serial.println(draw_per_brightness);
  Serial.print("max_brightness: ");
  Serial.println(max_brightness);
  
  return max_brightness;
}


void RampUpToMaxBrightness() {
  int max_brightness = DetermineMaxBrightness();
  
  for (int b = 0; b < max_brightness; b += 2) {
    Serial.println(b);
    strip.setBrightness(b);
    for (int i = 0; i < NUM_LIGHTS; i++) {
      strip.setPixelColor(i, WHITE);
    }
    strip.show();
    delay(1500);
  }
}


void RampLightsOut() {
  Serial.println("How many lights @255 Brightness:");

  int count = 1;
  while (count > 0) {
    // REP (Read, Evaluate, Print) loop
    if (!Serial.available()) {
      delay(10);
      continue;
    }
    
    count = Serial.parseInt();
    
    Serial.println("Asked to turn on ");
    Serial.println(count);

    strip.setBrightness(255);

    strip.clear();
    for (int i = 0; i < count; i++) {
      strip.setPixelColor(i, WHITE);
    }
    strip.show();
  }

  strip.clear();
  strip.setBrightness(10);
}

void setup() {
  // So serial monitor can be openned
  Serial.begin(9600);
  delay(5000);
  
  strip.begin();
  strip.clear();
  strip.show(); // Initialize all pixels to 'off'
}


void loop() {
//  RampUpToMaxBrightness();

  RampLightsOut();
}
