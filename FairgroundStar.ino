/*
    FairgroundStar. Firmware for an Arduino-based star-shaped fairground/circus-style LED light.
    Copyright (C) 2015  Tim Davis

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <FastLED.h>
#include <avr/sleep.h>

#include "config.h"
#include "Mood.h"
#include "MoodFairground.h"
#include "MoodTwinkle.h"
#include "MoodSteady.h"


// Define the array of leds that the moods write to
CRGB leds[NUM_LEDS];

// and the array of actual colours that FastLED uses.
// leds is used to update realLeds every frame, fading from
// the current colour (realLeds) to the required colour (leds).
// This is to simulate the response time of incandesent bulbs.
CRGB realLeds[NUM_LEDS];

Mood* curMood;
uint8_t curMoodNum = 0;
volatile uint32_t touchTime = 0;
volatile bool stillTouching = false;

/////////////////////////////////////////

inline void changeMood(void) {

  // clear down all leds
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  if (curMood) delete curMood;

  // cycle through moods
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

// interrupt handler for touch sensor
void touch(void) {
  
  // simply record that a touch event has started
  touchTime = millis();
  stillTouching = true;
}


/////////////////////////////////////////

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


/////////////////////////////////////////

void setup(void) {

  // initialise FastLED
  FastLED.addLeds<LED_TYPE, DATA_PIN, RGB_ORDER>(realLeds, NUM_LEDS);

  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setDither(DISABLE_DITHER);
  FastLED.setTemperature(Tungsten100W);

  // clear down any power-on led randomness asap
  FastLED.clear();
  FastLED.show();

  curMood = new MoodFairground();

  // set up handler for touch sensor
  pinMode(TOUCH_PIN, INPUT);
  attachInterrupt(TOUCH_INT, touch, RISING);

  // debugging
  /*
  Serial.begin(57600);

  Serial.print("free ram ");
  Serial.println(freeRam());
  Serial.flush();
  */
}


/////////////////////////////////////////

void loop(void) {
  
  static uint32_t lastRenderTime = 0;

  random16_add_entropy( random());
  
  // check if we're in the middle of a touch event
  if (stillTouching) {
    // if touch ended before reaching LONG_TOUCH_TIME
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

/////////////////////////////////////////
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}


