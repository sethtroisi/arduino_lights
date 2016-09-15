#include <Adafruit_NeoPixel.h>

#define PIN_NUMBER 3
#define NUM_LIGHTS 300
#define BRIGHTNESS 32


#define START_LED 0
#define END_LED 0


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN_NUMBER, NEO_GRB + NEO_KHZ800);

// Definition of some Colors which are used multiple times
uint32_t RED    = strip.Color(255, 0, 0);
uint32_t GREEN  = strip.Color(0, 255, 0);
uint32_t BLUE   = strip.Color(0, 0, 255);
uint32_t PURPLE = strip.Color(32, 0, 88);

uint32_t BLACK  = strip.Color(0, 0, 0);
uint32_t WHITE  = strip.Color(255, 255, 255);


// List (called Array in C) containing index of the first LED in each circle
int start_circle_index[] = {0, 70, 170}; // to extend use this format {5, 65, 140};

// List containing index index of the last LED in each cirle
int end_circle_index[] = {69, 139, 202};


void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}


int iteration = 0;
void loop() {
  for (int led_index = START_LED; led_index < END_LED; led_index += 1) {
    strip.setPixelColor(led_index, GREEN);
    
    if (led_index % 5 == 0) {
      if (iteration % 2 == 0) {
        strip.setPixelColor(led_index, RED);
      } else {
        strip.setPixelColor(led_index, BLUE);
      }
    }
  }
  strip.show();

  for (int i = 0; i < 20 * 5; i++) {
    // We run tracers around the circles a couple of times

    // C magic to determine how many elements are in the list.
    int num_circles = sizeof(start_circle_index) / sizeof(*start_circle_index);
    for (int circle = 0; circle < num_circles; circle += 1) {
      int start_index = start_circle_index[circle];
      int end_index = end_circle_index[circle];
      int num_leds = end_index - start_index + 1; // Math for fenceposts says "add one"

      for (int j = start_index; j < end_index; j++) {
        strip.setPixelColor(j, BLACK);
      }

      strip.setPixelColor(start_index, PURPLE);
      strip.setPixelColor(end_index, PURPLE);

      // Which LED in the circle to light up.
      int current_circle_index = i % num_leds;
      
      strip.setPixelColor(start_index + current_circle_index, WHITE);
    }

    strip.show(); 

    delay(50);
  }
  
  iteration += 1;
}
