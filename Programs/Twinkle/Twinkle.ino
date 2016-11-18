#include <Adafruit_NeoPixel.h>
#include <LightConfig.h>
#include <Colors.h>
#include <math.h>

#define PIN_NUMBER  LIGHT_CONFIG_PIN_NUM
#define NUM_LIGHTS  LIGHT_CONFIG_NUM_LIGHTS
#define BRIGHTNESS  LIGHT_CONFIG_BRIGHTNESS

// TODO(ERIN): move to LIGHT_CONFIG
#define NUM_TWINKLES 1
#define TWINKLE_LENGTH_MS 20 
#define MAX_DELAY_MS    50

#define num_elements(x)  (sizeof(x) / sizeof((x)[0]))

char twinkle_location[NUM_LIGHTS] = {};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN_NUMBER, NEO_GRB + NEO_KHZ800);


void setup() { 
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.clear();
  strip.show(); // Initialize all pixels to 'off'
}


void loop() {
  for (int t = 0; t < NUM_TWINKLES; t += 1) {
    // Set a "twinkle" at random points on twinkle_location for each pass of loop
    int set_twinkle = random(NUM_LIGHTS);
    twinkle_location[set_twinkle] = 20;
  }

  for (int l = 0; l < NUM_LIGHTS; l++) {
    if (twinkle_location[l] > 0) {
      uint32_t picker = random(num_elements(cool_colors));
      uint32_t light_color = cool_colors[picker];

      uint32_t faded;
      if (twinkle_location[l] > 10) {
        faded = LinearColorFade(light_color, BLACK, pow((20 - twinkle_location[l]) / 10.0, 0.2));
      } else {
        faded = LinearColorFade(light_color, BLACK, pow(twinkle_location[l] / 10.0, 0.2));
      }

      strip.setPixelColor(l, faded);
      twinkle_location[l] -= 1;
    } else {
      strip.setPixelColor(l, BLACK);
    }
  }
  strip.show(); 
  delay(random(MAX_DELAY_MS));
}



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

  return Adafruit_NeoPixel::Color(new_r, new_g, new_b);
}
