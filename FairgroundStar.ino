#include <FastLED.h>
#include <avr/sleep.h>

#include "Mood.h"
#include "MoodFairground.h"
#include "MoodTwinkle.h"
#include "MoodSteady.h"

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
CRGB leds[NUM_LEDS];
CRGB realLeds[NUM_LEDS];

Mood* curMood;
uint8_t curMoodNum = 0;
volatile uint32_t touchTime = 0;
volatile bool stillTouching = false;

/////////////////////////////////////////

inline void changeMood(void) {

  fill_solid(leds, NUM_LEDS, CRGB::Black);

  if (curMood) delete curMood;

  curMoodNum = (curMoodNum + 1) % 3;

  switch (curMoodNum) {

    case 0:
      curMood = new MoodFairground();
      break;
    case 1:
      curMood = new MoodTwinkle();
      break;
    case 2:
      curMood = new MoodSteady();
      break;
  }
}

/////////////////////////////////////////

void touch(void) {
  touchTime = millis();
  stillTouching = true;
}

/////////////////////////////////////////

void setup(void) {

  //Serial.begin(57600);
  
  // initialise FastLED
  FastLED.addLeds<LED_TYPE, DATA_PIN, RGB_ORDER>(realLeds, NUM_LEDS);

  // clear down any power-on led randomness asap
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setDither(DISABLE_DITHER);
  FastLED.setTemperature(Tungsten100W);
  FastLED.clear();
  FastLED.show();

  curMood = new MoodFairground();

  // set up handler for touch sensor
  pinMode(TOUCH_PIN, INPUT);
  attachInterrupt(TOUCH_INT, touch, RISING);
}


/////////////////////////////////////////

void loop(void) {
  
  static uint32_t lastRenderTime = 0;

  random16_add_entropy( random());
  
  // check if we're in the middle of a touch event
  if (stillTouching) {
    if (digitalRead(TOUCH_PIN) == LOW) {
      // short press - change pattern
      stillTouching = false;
      changeMood();
    }
    else if (millis() - touchTime > LONG_TOUCH_TIME) {
      // long press - turn off
      stillTouching = false;
      FastLED.clear();
      FastLED.show();

      // wait until touch input is off
      while (digitalRead(TOUCH_PIN) == HIGH);
      
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);    
      sleep_enable();
      sleep_mode();
      
      // The program will continue from here.
      // First thing to do is disable sleep.
      sleep_disable();
    }
  }
  
  // run rendering once per FRAME_TIME ms
  if (millis() - lastRenderTime >= FRAME_TIME) {
  
    lastRenderTime = millis();
    curMood->run();
    copyLedsToRealLeds(curMood->fadeStep());    
    FastLED.show();
  }
}

inline void copyLedsToRealLeds(uint8_t step) {
  // copy leds to realLeds with hysteresis
  for (uint8_t i=0; i<NUM_LEDS; i++) {
    CRGB *current = &realLeds[i];
    CRGB *target = &leds[i];

    // red    
    if (target->r != current->r) {
      if (target->r > current->r) {
        if (target->r - current->r > step) current->r += step;
        else current->r = target->r;
      }
      else {
        if (current->r - target->r > step) current->r -= step;
        else current->r = target->r;
      }
    }

    // green
    if (target->g != current->g) {
      if (target->g > current->g) {
        if (target->g - current->g > step) current->g += step;
        else current->g = target->g;
      }
      else {
        if (current->g - target->g > step) current->g -= step;
        else current->g = target->g;
      }
    }

    // blue
    if (target->b != current->b) {
      if (target->b > current->b) {
        if (target->b - current->b > step) current->b += step;
        else current->b = target->b;
      }
      else {
        if (current->b - target->b > step) current->b -= step;
        else current->b = target->b;
      }
    }
  }
}
