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

#ifndef __INC_MOOD_TWINKLE_H
#define __INC_MOOD_TWINKLE_H

#include "Mood.h"
#include "Palettes.h"
#include "Groups.h"
#include "config.h"


#define PALETTE_DURATION 30000
#define SEQ_DURATION 300000

#define NUM_TWINKLE 11
#define NUM_FADE 6
#define NUM_SEQ 3

struct Bulb {
  bool fadingUp;
  uint8_t value;
  uint8_t step;
  uint8_t paletteIndex;
  CRGB colour;
  uint8_t setNum;
  
  void init(uint8_t step, uint8_t curPalette, uint8_t paletteIndex, uint8_t setNum) {
    this->fadingUp = true;
    this->value = random8();
    this->step = step;
    this->paletteIndex = paletteIndex;
    this->colour = palettes[curPalette].colours[paletteIndex];
    this->setNum = setNum;
  }
  
  void update(uint8_t curPalette, uint8_t curGroup, bool cycleIndex) {

    // update value
    if (fadingUp) {
      value = qadd8(value, step);
      if (value == 255) {
        fadingUp = false;
      }
    }
    else {
      value = qsub8(value, step);
      if (value == 0) {
        fadingUp = true;
        if (cycleIndex) {
          paletteIndex = (paletteIndex + 1) % palettes[curPalette].size;
        }
        // re-get colour from palette even if not cycling in case palette has changed
        colour = palettes[curPalette].colours[paletteIndex];        
      }
    }
    
    // render
    
    // get current colour
    CRGB col = colour;
    
    // scale by value
    col %= pow8(value, 2);
    
    // set relevant leds
    twinkleGroups[curGroup].setColour(setNum, col);
  }
  
  // raise n to the power e, treating n as a fraction from 0/256 to 255/256
  inline uint8_t pow8(uint8_t n, uint8_t e) {
    uint8_t result = n;
    while (e > 1) {
      result = scale8(result, n);
      e--;
    }
    return result;
  }

};


class MoodTwinkle : public Mood {
  
  uint8_t curPalette;
  uint8_t curPaletteIndex;
  bool cycleIndex;
  
  uint8_t curSeq;
  
  uint32_t lastPaletteChangeTime;
  uint32_t lastSeqChangeTime;

  Bulb bulbs[NUM_LEDS];

public:
  MoodTwinkle() {
    lastPaletteChangeTime = lastSeqChangeTime = millis();
    curSeq = random8(0, NUM_SEQ);
    changePalette();
    init();    
  }
  
  bool run(void) {
    // time to change palette?
    if (millis() - lastPaletteChangeTime > PALETTE_DURATION) {
      lastPaletteChangeTime = millis();
      changePalette();
      
      if (!cycleIndex) {
        setBulbPaletteIndexToCurrent();
      }
    }
    
    if (millis() - lastSeqChangeTime > SEQ_DURATION) {
      lastSeqChangeTime = millis();
      uint8_t lastSeq = curSeq;

      // pick a new sequence that is different to the current one
      do {
        curSeq = random8(0, NUM_SEQ);
      } while (curSeq == lastSeq);
      
      init();
    }
    
    switch (curSeq) {
      case 0: // twinkle
      case 1: // slow twinkle
        for (int8_t i=0; i<NUM_TWINKLE; i++) {
          bulbs[i].update(curPalette, GROUP_TWINKLE_INDIVIDUAL, cycleIndex);
        }
        break;
        
      case 2: // fade
        for (int8_t i=0; i<NUM_FADE; i++) {
          bulbs[i].update(curPalette, GROUP_TWINKLE_ARMS, cycleIndex);
        }
        break;        
    }
    
    return true;
  }
 
 private:
 
  void changePalette(void) {
    curPalette = twinklePalettes[random8(NUM_TWINKLE_PALETTES)];
    curPaletteIndex = random8(0, palettes[curPalette].size);
    cycleIndex = (random8() < 180);
  }
  
  void init(void) {
    uint8_t stepMin;
    switch (curSeq) {
      case 0: // twinkle
      case 1: // slow twinkle
        for (int8_t i=0; i<NUM_LEDS; i++) {
          bulbs[i].init( (curSeq == 0 ? 15 : 6) + i, curPalette, curPaletteIndex, i);
        }
        break;
  
      case 2: // fade
        for (int8_t i=0; i<NUM_LEDS; i++) {
          bulbs[i].init(2 + i%2, curPalette, curPaletteIndex, i);
        }
        break;      
    }    
  }
  
  void setBulbPaletteIndexToCurrent(void) {
    for (int8_t i=0; i<NUM_LEDS; i++) {
      bulbs[i].paletteIndex = curPaletteIndex;
    }
  }
};

#endif

