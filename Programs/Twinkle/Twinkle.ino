#include <Adafruit_NeoPixel.h>
#include <LightConfig.h>
#include <Colors.h>
#include <Math.h>

#define PIN_NUMBER  LIGHT_CONFIG_PIN_NUM
#define NUM_LIGHTS  LIGHT_CONFIG_NUM_LIGHTS
#define BRIGHTNESS  LIGHT_CONFIG_BRIGHTNESS

// TODO(ERIN): move to LIGHT_CONFIG
#define DELAY_MS    120

#define num_elements(x)  (sizeof(x) / sizeof((x)[0]))

double density = 0.03; //average percent of lights turned on
char twinkle_location[NUM_LIGHTS] = {};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN_NUMBER, NEO_GRB + NEO_KHZ800);


void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.clear();
  strip.show(); // Initialize all pixels to 'off'
}


void loop() {

  for (int x = 0; x < NUM_LIGHTS; x++) {
    if (random(10000) < density / 20 * 10000 && twinkle_location[x] == 0) {
      twinkle_location[x] = 19;
    }
  }

  for (int light = 0; light < NUM_LIGHTS; light++) {
    if (twinkle_location[light] > 0) {
      uint32_t picker = light % (num_elements(xmas_colors));
      uint32_t light_color = xmas_colors[picker];
      uint32_t fade_color;
      if (twinkle_location[light] > 15) {
        fade_color = LinearColorFade(BLACK, light_color, pow(((20 - twinkle_location[light])) / 5.0, 2));
      }
      else {
        fade_color = LinearColorFade(BLACK, light_color, pow(twinkle_location[light] / 15.0, 0.95));
      }
      strip.setPixelColor(light, fade_color);
      twinkle_location[light] -= 1;
    }
    else {
      strip.setPixelColor(light, BLACK);
    }

  }
  strip.show();
  delay(DELAY_MS);
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
