/**************************************************************************/
/*
    @file     LightConfig.h
    @author   Seth T. & Erin D.

    This is a library for compromise.
    One of us likes to have 150 lights. One of us prefers 300.
    I like Red to have a tint of Blue. They like unadultered Red.
    It's small things like this that keep our code together.

    See https://gist.github.com/canton7/1423106 for original idea
*/
/**************************************************************************/

#define LIGHT_CONFIG_PIN_NUM		3

#define LIGHT_CONFIG_NUM_LIGHTS		300
#define LIGHT_CONFIG_BRIGHTNESS		32


#define LIGHT_CONFIG_INTERSECTIONS {\
  {  1, 69, 70, 141, 298},\
  {  102, 169, 170, 239, 241},\
  {  37, 202, 269, 270}\
};
