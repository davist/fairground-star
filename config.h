#ifndef __INC_CONFIG_H
#define __INC_CONFIG_H

#include <FastLED.h>

// led strip
#define LED_TYPE WS2812B
#define RGB_ORDER GRB
#define NUM_LEDS 11
#define DATA_PIN 12

// animation speed in frames per second
#define FPS 30
// time per frame in ms
#define FRAME_TIME (1000/(FPS))

// touch sensor
#define TOUCH_PIN 2
#define TOUCH_INT 0

#define LONG_TOUCH_TIME 1000


// Define the array of leds
extern CRGB leds[NUM_LEDS];

#endif

