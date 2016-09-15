#include <Adafruit_NeoPixel.h>

#define PIN_NUMBER 3
#define NUM_LIGHTS 150
#define BRIGHTNESS 32

#define DELAY_MS 65

#define num_elements(x)  (sizeof(x) / sizeof((x)[0]))

int intersections[][6] = {
  {  15,  16},
  {  29,  30, 107, 108, 109},
  {  91,  92},
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


uint32_t fun_colors[] = {RED, GREEN, BLUE, PURPLE, BLACK, WHITE};

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
      for (int j = 0; true; j++) {
        int led_index = intersection_points[j];
        if (led_index <= 0) {
          break;
        }
        
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
    for (int snake_index = 0; snake_index < 1; snake_index += 1) {
      strip.setPixelColor(snakes[snake_index], BLACK);
      snakes[snake_index] = (snakes[snake_index] + 1) % NUM_LIGHTS;
      strip.setPixelColor(snakes[snake_index], fun_colors[snake_index]);
    }

    strip.show();
    delay(DELAY_MS);
  }

}
