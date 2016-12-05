#include <Adafruit_NeoPixel.h>
#include <LightConfig.h>
#include <Colors.h>
#include <Math.h>

#define PIN_NUMBER  LIGHT_CONFIG_PIN_NUM
#define NUM_LIGHTS  LIGHT_CONFIG_NUM_LIGHTS
#define BRIGHTNESS  LIGHT_CONFIG_BRIGHTNESS

// TODO(ERIN): move to LIGHT_CONFIG
#define DELAY_MS   90

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
  
  for (int a = 0; a < 100; a++) {
    
    for (int y = 0; y < arc_size; y++) {
      for (int x = 0; x < num_arcs; x++) {
        int start_point_arc = start_arc_index[x]; 
        strip.setPixelColor(start_arc_index[x] + y, xmas_colors[a % num_elements(xmas_colors)]);
      }
      strip.show();
      delay(DELAY_MS);
    }

      
  } // int a loop close
    
    
} // void loop close





