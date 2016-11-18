/**************************************************************************/
/*! 
    @file     Colors.h
    @author   Seth T. & Erin D.

    This is a central place to define all the pretty colors.
*/
/**************************************************************************/
#include <Adafruit_NeoPixel.h>

uint32_t BLACK  = Adafruit_NeoPixel::Color(0, 0, 0);
uint32_t WHITE  = Adafruit_NeoPixel::Color(255, 255, 255);

uint32_t RED      = Adafruit_NeoPixel::Color(255, 0, 0);
uint32_t GREEN    = Adafruit_NeoPixel::Color(0, 255, 0);
uint32_t BLUE     = Adafruit_NeoPixel::Color(0, 0, 255);

uint32_t YELLOW   = Adafruit_NeoPixel::Color(255, 255, 0);
uint32_t MAGENTA   = Adafruit_NeoPixel::Color(255, 0, 255);
uint32_t CYAN   = Adafruit_NeoPixel::Color(0, 255, 255);

uint32_t ROSE   = Adafruit_NeoPixel::Color(255, 0, 200);
uint32_t ORANGE   = Adafruit_NeoPixel::Color(255, 200, 0);
uint32_t LIME   = Adafruit_NeoPixel::Color(200, 255, 0);
uint32_t VIOLET   = Adafruit_NeoPixel::Color(200, 0, 255);
uint32_t TEAL   = Adafruit_NeoPixel::Color(0, 255, 200);
uint32_t COBALT   = Adafruit_NeoPixel::Color(0, 200, 255);

uint32_t PURPLE   = Adafruit_NeoPixel::Color(80, 0, 220);
uint32_t PLUM   = Adafruit_NeoPixel::Color(150, 0, 204);
uint32_t BERRY   = Adafruit_NeoPixel::Color(219, 0, 88);
uint32_t SKY   = Adafruit_NeoPixel::Color(0, 168, 165);

uint32_t all_colors[] = {WHITE, RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, ROSE, ORANGE, LIME, VIOLET, TEAL, COBALT, PURPLE, PLUM, BERRY, SKY};

uint32_t cool_colors[] = {GREEN, BLUE, CYAN, LIME, VIOLET, TEAL, COBALT, PURPLE, PLUM, SKY};

uint32_t warm_colors[] = {RED, YELLOW, MAGENTA, ROSE, ORANGE, BERRY};
