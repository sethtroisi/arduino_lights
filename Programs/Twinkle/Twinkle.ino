#include <Adafruit_NeoPixel.h>
#include <LightConfig.h>
#include <Colors.h>

#define PIN_NUMBER  LIGHT_CONFIG_PIN_NUM
#define NUM_LIGHTS  LIGHT_CONFIG_NUM_LIGHTS
#define BRIGHTNESS  LIGHT_CONFIG_BRIGHTNESS

// TODO(ERIN): move to LIGHT_CONFIG
#define NUM_TWINKLES 8
#define TWINKLE_LENGTH_MS 20
#define MAX_DELAY_MS    50

#define num_elements(x)  (sizeof(x) / sizeof((x)[0]))

int twinkle_location[NUM_LIGHTS] = {};

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
      twinkle_location[set_twinkle] = 1;
      }

    for (int l = 0; l < NUM_LIGHTS; l++) {
        if (twinkle_location[l] == 1) {
          uint32_t picker = random(num_elements(cool_colors));
          uint32_t light_color = cool_colors[picker];
          strip.setPixelColor(l, light_color);
          delay(TWINKLE_LENGTH_MS);
          twinkle_location[l] = 0;
        } else {
          strip.setPixelColor(l, BLACK);
        }
        strip.show(); 
     }
     delay(random(MAX_DELAY_MS)); 
  }

