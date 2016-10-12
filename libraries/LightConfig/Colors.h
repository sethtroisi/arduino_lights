/**************************************************************************/
/*! 
    @file     Colors.h
    @author   Seth T. & Erin D.

    This is a central place to define all the pretty colors.
*/
/**************************************************************************/

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN_NUMBER, NEO_GRB + NEO_KHZ800);

uint32_t BLACK  = strip.Color(0, 0, 0);
uint32_t WHITE  = strip.Color(255, 255, 255);

uint32_t RED      = strip.Color(255, 0, 0);
uint32_t GREEN    = strip.Color(0, 255, 0);
uint32_t BLUE     = strip.Color(0, 0, 255);

uint32_t YELLOW   = strip.Color(255, 255, 0);
uint32_t MAGENTA   = strip.Color(255, 0, 255);
uint32_t CYAN   = strip.Color(0, 255, 255);

uint32_t ROSE   = strip.Color(255, 0, 200);
uint32_t ORANGE   = strip.Color(255, 200, 0);
uint32_t LIME   = strip.Color(200, 255, 0);
uint32_t VIOLET   = strip.Color(200, 0, 255);
uint32_t TEAL   = strip.Color(0, 255, 200);
uint32_t COBALT   = strip.Color(0, 200, 255);

uint32_t PURPLE   = strip.Color(80, 0, 220);
uint32_t PLUM   = strip.Color(150, 0, 204);
uint32_t BERRY   = strip.Color(219, 0, 88);
uint32_t SKY   = strip.Color(0, 168, 165);



