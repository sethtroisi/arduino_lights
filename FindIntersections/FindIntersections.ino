#include <Adafruit_NeoPixel.h>
#include <LightConfig.h>

#define PIN_NUMBER  LIGHT_CONFIG_PIN_NUM
#define NUM_LIGHTS  LIGHT_CONFIG_NUM_LIGHTS
#define BRIGHTNESS  LIGHT_CONFIG_BRIGHTNESS

#define DELAY_MS    35

#define num_elements(x)  (sizeof(x) / sizeof((x)[0]))

int intersections[][6] = LIGHT_CONFIG_INTERSECTIONS


int snakes [] = {
  10,
  20,
  30,
  40,
  50,
  60,
  70,
};

int can_jump[] = {
  0,
  0,
  0,
};



Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN_NUMBER, NEO_GRB + NEO_KHZ800);

// Definition of some Colors which are used multiple times
uint32_t RED    = strip.Color(255, 0, 0);
uint32_t GREEN  = strip.Color(0, 255, 0);
uint32_t BLUE   = strip.Color(0, 0, 255);
uint32_t PURPLE = strip.Color(32, 0, 88);

uint32_t BLACK  = strip.Color(0, 0, 0);
uint32_t WHITE  = strip.Color(255, 255, 255);

uint32_t fun_colors[] = {PURPLE, WHITE, GREEN, PURPLE, BLACK, WHITE};

void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}


void loop() {
  for (int f = 0; f < 10; f++) {
    int num_intersections = num_elements(intersections);
    for (int i_index = 0; i_index < num_intersections; i_index += 1) {
      int *intersection_points = intersections[i_index];
      for (int j = 0; intersection_points[j] > 0; j++) {
        int led_index = intersection_points[j];        
        strip.setPixelColor(led_index, fun_colors[(f + i_index + j) % num_elements(fun_colors)]);
      }
    }

    strip.show();
    delay(500);
  }


  // Black all LEDs
  for (int i = 0; i < NUM_LIGHTS; i += 1) {
    strip.setPixelColor(i, BLACK);
  }


  for (int f = 0; f < 10000; f++) {

    int num_snakes = num_elements(snakes);
    for (int snake_index = 0; snake_index < num_snakes; snake_index += 1) {
      // For each snake
      int current_led = snakes[snake_index];

      // Turn off it's current led.
      strip.setPixelColor(current_led, BLACK);

      int next_led = (current_led + 1) % NUM_LIGHTS;

      // see if this is an intersection
      int n_i = num_elements(intersections);
      for (int i_index = 0; i_index < n_i; i_index += 1) {
        int *i_points = intersections[i_index];
          
        for (int j = 0; j < i_points[j] > 0; j++) {
          int led_index = i_points[j];
  
          if (led_index == current_led) {
            // This led is part of an intersection
            // If the light hasn't jumped recently then
            // 50% of the time jump to one of the other LEDS in the intersection.
            if (can_jump[snake_index] > 0 && random(2) == 0) {
              for (int k = 0; i_points[k] > 0; k += 1) {
                // Find an led that's not on this "strand" (ie not an adjacent led)
                int test_led = i_points[k];
                if (abs(current_led - test_led) > 3) {
                  next_led = test_led;
                  can_jump[snake_index] = -10; // Don't jump for a while
                }
              }
            }
          }
        }
      }

      // Turn on the new led!
      strip.setPixelColor(next_led, fun_colors[snake_index % num_elements(fun_colors)]);
      snakes[snake_index] = next_led;
      can_jump[snake_index] += 1;
    }

    strip.show();
    delay(DELAY_MS);
  }

}
