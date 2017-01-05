#include <Adafruit_NeoPixel.h>
#include <LightConfig.h>
#include <Colors.h>
#include <math.h>

#define PIN_NUMBER  LIGHT_CONFIG_PIN_NUM
#define NUM_LIGHTS  LIGHT_CONFIG_NUM_LIGHTS
#define BRIGHTNESS  LIGHT_CONFIG_BRIGHTNESS

#define DELAY_MS 17 // Aim for 50hz

#define num_elements(x)  (sizeof(x) / sizeof((x)[0]))

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN_NUMBER, NEO_GRB + NEO_KHZ800);
uint32_t color_history[NUM_LIGHTS] = {};

void setup() {  
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.clear();
  strip.show(); // Initialize all pixels to 'off'

  for (int i = 0; i < NUM_LIGHTS; i++) {
    color_history[i] = BLACK;
  }
}


float randomFloat() {
  return random(1000001) / 1.0e6;
}


// integrate the PID with props and stuff.
float update(float a, float minA, float maxA, float deltaAmount, float prop) {
  float r = randomFloat();
  
  a += (r < prop) ? deltaAmount : -deltaAmount;
  
  return constrain(a, minA, maxA);
}


// These are globals because I'm a bad programmer :)
float current_center = 0;
float velocity = 0.5;
float velocityProp = 0.0;

int madHatter() {
  // "Random" walk but ~~~~ 3rd order PID

  int light_range = NUM_LIGHTS / 2 - 5;

  // Derivative term on PID, unbalanced negatively.
  velocityProp   = update(velocityProp,    -0.4,           0.4,           0.01,     0.5 + 0.5 * (current_center / light_range) ); 
  velocity       = update(velocity,        -1.0,           1.0,           0.05,     0.5 + velocityProp);
  current_center = update(current_center,  -light_range,   light_range,  velocity,  0.5);

  return 5 + (current_center + light_range);
}


uint32_t cur_color;
int iteration = 0;
void loop() {
  iteration += 1;

  if (iteration % 200 == 1) { // || random(1000) == 0) {
    cur_color = all_colors[random(num_elements(all_colors))];
  }

  int location = madHatter();
  color_history[location] = cur_color;


  uint32_t new_color;
  for (int l = 1; l < NUM_LIGHTS; l++) {
    // Fade closer colors more!
    float fade = 0.01 * (max(location, NUM_LIGHTS - location) - abs(l - location));

    if (randomFloat() < fade) {
      // if chance happens fade a little towards black;
      color_history[l] = LinearColorFade(color_history[l], BLACK, 0.05);
    }
    strip.setPixelColor(l, color_history[l]);
  }

  strip.show();
  delay(DELAY_MS);
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


