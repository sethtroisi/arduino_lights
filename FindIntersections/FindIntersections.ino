#include <Adafruit_NeoPixel.h>
#include <LightConfig.h>

#define PIN_NUMBER  LIGHT_CONFIG_PIN_NUM
#define NUM_LIGHTS  LIGHT_CONFIG_NUM_LIGHTS
#define BRIGHTNESS  LIGHT_CONFIG_BRIGHTNESS

// TODO(ERIN): move to LIGHT_CONFIG
#define NUM_SNAKES  6
#define TAILS       7
#define DELAY_MS    35

#define num_elements(x)  (sizeof(x) / sizeof((x)[0]))

short intersections[][6] = LIGHT_CONFIG_INTERSECTIONS

int snake_location[NUM_SNAKES] = {};
int snake_direction[NUM_SNAKES] = {};
int snake_can_jump[NUM_SNAKES] = {};
short snake_tails[NUM_SNAKES][TAILS] = {};


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN_NUMBER, NEO_GRB + NEO_KHZ800);

// Definition of some Colors which are used multiple times
uint32_t RED      = strip.Color(255, 0, 0);
uint32_t YELLOW   = strip.Color(255, 255, 0);
uint32_t ORANGE   = strip.Color(255, 165, 0);
uint32_t GREEN    = strip.Color(0, 255, 0);
uint32_t BLUE     = strip.Color(0, 0, 255);
uint32_t PURPLE   = strip.Color(80, 0, 220);

uint32_t BLACK  = strip.Color(0, 0, 0);
uint32_t WHITE  = strip.Color(255, 255, 255);

uint32_t fun_colors[] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, BLACK, WHITE};

void setup() { 
  for (int s = 0; s < NUM_SNAKES; s++) {
    snake_location[s] = 0;
    snake_direction[s] = 1;
    snake_can_jump[s] = 0;

    for (int t = 0; t < TAILS; t++) {
      snake_tails[s][t] = 0;
    }
  }
  
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  
  strip.clear();
  strip.show(); // Initialize all pixels to 'off'
}


void loop() {

/*
  // Show the intersections
  strip.clear();
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
*/

  strip.clear();
  for (int f = 0; f < 10000; f++) {
    int num_snakes = num_elements(snake_location);
    for (int snake_index = 0; snake_index < num_snakes; snake_index += 1) {
      // For each snake
      int current_led = snake_location[snake_index];

      int dir = snake_direction[snake_index];
      int next_led = (current_led + dir) % NUM_LIGHTS;
      // C allows negative on modulo.
      if (next_led < 0) {
        next_led += NUM_LIGHTS;
      }

      // see if this is an intersection
      int n_i = num_elements(intersections);
      for (int i_index = 0; i_index < n_i; i_index += 1) {
        short *i_points = intersections[i_index];
          
        for (int j = 0; j < i_points[j] > 0; j++) {
          if (i_points[j] == current_led) {
            // This led is part of an intersection
            // If the light hasn't jumped recently then
            // 50% of the time jump to one of the other LEDS in the intersection.
            if (snake_can_jump[snake_index] > 0 && random(2) == 0) {
              for (int k = 0; i_points[k] > 0; k += 1) {
                // Find an led that's not on this "strand" (ie not an adjacent led)
                int test_led = i_points[k];
                if (abs(current_led - test_led) > 3) {
                  next_led = test_led;
                  snake_can_jump[snake_index] = -10; // Don't jump for a while
                  snake_direction[snake_index] = 2 * random(2) - 1; // 0,2 => -1 or 1
                }
              }
            }
          }
        }
      }


      // Turn off it's current led.
      for (int t = 0; t < TAILS; t++) {
        strip.setPixelColor(snake_tails[snake_index][t], BLACK);
      }


      // Set new tail indexes
      for (int t = TAILS-2; t >= 0; t -= 1) {
        snake_tails[snake_index][t+1] = snake_tails[snake_index][t];
      }

      snake_tails[snake_index][0] = next_led;      
      snake_location[snake_index] = next_led;

      // Turn on leds in the tail
      uint32_t light_color = fun_colors[snake_index % num_elements(fun_colors)];
      for (int t = 0; t < TAILS; t++) {
        float percent = (TAILS - t + 0.0) / TAILS;
        percent = percent * percent;
        strip.setPixelColor(snake_tails[snake_index][t], LinearColorFade(BLACK, light_color, percent));
      }
      snake_can_jump[snake_index] += 1;
    }

    strip.show();
    delay(DELAY_MS);
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

  return strip.Color(new_r, new_g, new_b);
}
