#include <Adafruit_NeoPixel.h>
#include <LightConfig.h>
#include <Colors.h>
#include <Math.h>

#define PIN_NUMBER  LIGHT_CONFIG_PIN_NUM
#define NUM_LIGHTS  LIGHT_CONFIG_NUM_LIGHTS
#define BRIGHTNESS  LIGHT_CONFIG_BRIGHTNESS

// TODO(ERIN): move to LIGHT_CONFIG
#define NUM_TWINKLES 15
#define DELAY_MS    20

#define num_elements(x)  (sizeof(x) / sizeof((x)[0]))


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN_NUMBER, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.clear();
  strip.show(); // Initialize all pixels to 'off'
}


#define LIGHT_CIRCLES {\
  { 0, 69 },\
  { 70, 139},\
  {170, 239}\
}

int test[3][2] = LIGHT_CIRCLES;

int iter = 0;
void loop() {
  iter++; 

  strip.clear();
/*
  for (int circle = 0; circle < 3 num_elements(test); circle++) {
    int start_index = test[circle][0];
    int   end_index = test[circle][1];
    uint32_t light_color = xmas_colors[circle];
)
    int inner_iter = (iter % 100);
    int num_circle = end_index - start_index + 1;
    for (int i = 0; i < num_circle; i++) {
      if ((i * 100) < (num_circle * inner_iter)) {
        strip.setPixelColor(start_index + i, light_color);
      }
    }    
  }
*/

  int CYCLES_PER_ROTATION = 50;
  int CYCLES_PER_PERCENT = 10;

  if (iter >= CYCLES_PER_ROTATION * CYCLES_PER_PERCENT) {
    iter = 0;
  }
  double percent = 100.0 * iter / CYCLES_PER_ROTATION / CYCLES_PER_PERCENT;

  for (int circle = 0; circle < num_elements(test); circle++) {
    uint32_t light_color = xmas_colors[circle];

    int start_index = test[circle][0];
    int   end_index = test[circle][1];

    int num_circle = end_index - start_index + 1;
    double start_percent = 100.0 * iter / CYCLES_PER_ROTATION;
    while (start_percent > 100) {
      start_percent -= 100;
    }
/*    
    if (circle == 0) {
      start_percent += 16;
      start_percent -= 100 * (start_percent > 100);
    }
    if (circle == 2) {
      start_percent += 32;
      start_percent -= 100 * (start_percent > 100);
    }
*/
    int start_count_index = start_percent * num_circle / 100;
    int count = num_circle * percent / 100;
    for (int i = 0; i < count; i++) {
      strip.setPixelColor(start_index + ((start_count_index + i) % num_circle), light_color);
    }    
  }
  
  strip.show();
  delay(DELAY_MS);
}

