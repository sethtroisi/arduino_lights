#include <Adafruit_NeoPixel.h>
#include <LightConfig.h>
#include <Colors.h>
#include <Math.h>

#define PIN_NUMBER  LIGHT_CONFIG_PIN_NUM
#define NUM_LIGHTS  LIGHT_CONFIG_NUM_LIGHTS
#define BRIGHTNESS  LIGHT_CONFIG_BRIGHTNESS

// TODO(ERIN): move to LIGHT_CONFIG
#define DELAY_MS   80

#define num_elements(x)  (sizeof(x) / sizeof((x)[0]))

char twinkle_location[NUM_LIGHTS] = {};
int start_arc_index[] = {141, 241, 270};
int end_arc_index[] = {169, 269, 298};
int start_circle_index[] = {0, 70, 170};
int end_circle_index[] = {69, 139, 239};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN_NUMBER, NEO_GRB + NEO_KHZ800);


void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.clear();
  strip.show(); // Initialize all pixels to 'off'
}


void loop() {

  int arc_size = end_arc_index[1] - start_arc_index[1];
  int num_arcs = num_elements(start_arc_index);
  int start_point_arc[num_arcs] = {};
  
  int circle_size = end_circle_index[1] - end_circle_index[1];
  int num_circles = num_elements(start_circle_index);
  int start_point_circle[num_circles] = {};
  
  for (int a = 0; a < 100; a++) {
    
    for (int y = 0; y < arc_size; y++) {
      for (int x = 0; x < num_arcs; x++) {
        int start_point_arc = start_arc_index[x]; 
        strip.setPixelColor(start_arc_index[x] + y, xmas_colors[a % num_elements(xmas_colors)]);
      }
      strip.show();
    delay(DELAY_MS);
  
    }
    
    
  }

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
