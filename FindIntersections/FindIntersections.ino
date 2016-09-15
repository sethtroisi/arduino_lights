#include <Adafruit_NeoPixel.h>

#define PIN_NUMBER 3
#define NUM_LIGHTS 300
#define BRIGHTNESS 32

#define DELAY_MS 35

#define num_elements(x)  (sizeof(x) / sizeof((x)[0]))

int intersections[][6] = {
  {  1,  69,  70,  141, 298},
  {  102,  169, 170, 239, 241},
  {  37, 202, 269, 270},
};


int snakes [] = {
  10,
  20,
  70,
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
      int current_led = snakes[snake_index];

      strip.setPixelColor(current_led, BLACK);

      int next_led = (current_led + 1) % NUM_LIGHTS;

      // see if this is an intersection
      int n_i = num_elements(intersections);
      for (int i_index = 0; i_index < n_i; i_index += 1) {
        int *i_points = intersections[i_index];
        for (int j = 0; i_points[j] > 0; j++) {
          int led_index = i_points[j];

          if (led_index == current_led) {
            // This led is part of an intersection
            // 50% of the time jump to one of the other LEDS in the intersection 
            if (random(0,2) == 0) {
              if (j == 0) {
                next_led = i_points[1] + 2; // move out of the intersection
              } else {
                next_led = i_points[j - 1] + 2;
              }
            }
          }
        }
      }

      strip.setPixelColor(next_led, fun_colors[snake_index]);
      snakes[snake_index] = next_led;
    }

    strip.show();
    delay(DELAY_MS);
  }

}
