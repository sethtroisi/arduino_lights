#include <Adafruit_NeoPixel.h>
#include <LightConfig.h>

#define PIN_NUMBER  LIGHT_CONFIG_PIN_NUM
#define NUM_LIGHTS  LIGHT_CONFIG_NUM_LIGHTS
#define BRIGHTNESS  LIGHT_CONFIG_BRIGHTNESS

#define num_elements(x)  (sizeof(x) / sizeof((x)[0]))

#define START_LED 0
#define END_LED 0

//COLORS
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN_NUMBER, NEO_GRB + NEO_KHZ800);
#include <Colors.h>


int start_inner_index[] = {141, 241, 270};
int end_inner_index[] = {169, 269, 298};

int start_outer_index[] = {103};
int end_outer_index[] = {142};

void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}


void loop() {
  for (int i = 141; i < 169; i++) {
    strip.setPixelColor(i, SKY);
    delay(50);
    strip.show();
    strip.setPixelColor(i, BLACK);
    } 
  for (int i = 103; i < 142; i++) {
    strip.setPixelColor(i, SKY);
    delay(50);
    strip.show();
    strip.setPixelColor(i, BLACK);
    } 
//
//  for (int i = 241; i < 269; i++) {
//    strip.setPixelColor(i, SKY);
//    delay(50);
//    strip.show();
//    } 
//  for (int i = 270; i < 298; i++) {
//    strip.setPixelColor(i, SKY);
//    delay(50);
//    strip.show();
//    } 
}
