#include <Adafruit_NeoPixel.h>
#include <LightConfig.h>
#include <Colors.h>
#include <math.h>

#include "Wave.h"

#define PIN_NUMBER  LIGHT_CONFIG_PIN_NUM
#define NUM_LIGHTS  LIGHT_CONFIG_NUM_LIGHTS
#define BRIGHTNESS  LIGHT_CONFIG_BRIGHTNESS

#define DELAY_MS 17 // Aim for 50hz

#define num_elements(x)  (sizeof(x) / sizeof((x)[0]))

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN_NUMBER, NEO_GRB + NEO_KHZ800);

void setup() {  
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.clear();
  strip.show(); // Initialize all pixels to 'off'
}


cord getCords(int i) {
  if ((18 <= i) && (i <= 90)) {
    float circle_size_metres = 0.4;
    
    float num_in_circle = 69;
    float angle_rads = M_PI / 2.0 - 2.0 * M_PI * (i - 22) / num_in_circle;

    return (cord) {
      circle_size_metres * cos(angle_rads),
      circle_size_metres * sin(angle_rads)
    };
  }

  if ((91 <= i) && (i <= 109)) {
    float line_size_metres = 0.4;
    float num_in_line = 22;

    float y = 0.5 - (i - 91) / num_in_line;

    return (cord) {
      0,
      line_size_metres * y
    };
  }

  if ((129 <= i) && (i <= 150)) {
    float line_size_metres = 0.4;
    float num_in_line = 20;
    
    float x = -0.5 + (i - 129) / num_in_line;

    return (cord) {
      line_size_metres * x,
      0
    };
  }

  cord cords = {-1.0, -1.0};
  return cords;
}


float randomFloat() {
  return random(1000001) / 1.0e6;
}


float line_m = 0;
float line_b = 0;
float line_b_inc = 0;
uint32_t cur_color = BLACK;

int iteration = 0;
void loop() {
  iteration += 1;

  if (iteration % 200 == 1) { // || random(1000) == 0) {
    // Set line_m to [-4, 4]
    // Set line_b_inc to [-5, 5] / 200

    line_m = 8 * randomFloat() - 4;
    line_b = 0;
    line_b_inc = (10 * randomFloat() - 5) / 50.0;

    cur_color = all_colors[random(num_elements(all_colors))];
  }
  
  line_b += line_b_inc;

  strip.setPixelColor(0, cur_color);
  for (int l = 1; l < NUM_LIGHTS; l++) {
    cord cords = getCords(l);

    uint32_t color = BLACK;
    if (cords.y < cords.x * line_m + line_b) {
      color = cur_color;
    };

    strip.setPixelColor(l, color);
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
