#ifndef __INC_PATTERN_CHRISTMAS_TREE_H
#define __INC_PATTERN_CHRISTMAS_TREE_H

#include "Pattern.h"
#include "Palettes.h"
#include "Groups.h"

#define NUM_LEDS 11

#define PALETTE_DURATION 30000
#define SEQ_DURATION 120000

#define NUM_TWINKLE 11
#define NUM_CHASE 11
#define NUM_FADE 6
#define NUM_SEQ 2

#define CHASE_DELAY 5
#define CHASE_GAP 3


struct Bulb {
  bool fadingUp;
  uint8_t value;
  uint8_t step;
  uint8_t paletteIndex;
  uint8_t setNum;
  
  void init(uint8_t step, uint8_t paletteIndex, uint8_t setNum) {
    this->fadingUp = true;
    this->value = random8();
    this->step = step;
    this->paletteIndex = paletteIndex;
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
      }
    }
    
    // render
    
    // get colour from palette
    CRGB col = palettes[curPalette].colours[paletteIndex];
    
    // scale by value
    col %= ease8InOutCubic(value);
    
    // set relevant leds
    groups[curGroup].setColour(setNum, col);
  }
};


class PatternChristmasTree : public Pattern {
  
  uint8_t curPalette;
  uint8_t curPaletteIndex;
  bool cycleIndex;
  
  uint8_t curSeq;
  
  uint32_t lastPaletteChangeTime;
  uint32_t lastSeqChangeTime;

  uint8_t chaseStart;
  uint8_t chaseDelayCount;

  Bulb bulbs[11];

public:
  PatternChristmasTree() {
    chaseDelayCount = CHASE_DELAY;

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
        init();
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
      
      chaseDelayCount = CHASE_DELAY;
    }
    
    switch (curSeq) {
      case 0: // twinkle
        for (int8_t i=0; i<NUM_TWINKLE; i++) {
          bulbs[i].update(curPalette, GROUP_SPIRAL, cycleIndex);
        }
        break;
        
      case 1: // fade
        for (int8_t i=0; i<NUM_FADE; i++) {
          bulbs[i].update(curPalette, GROUP_ARMS2, cycleIndex);
        }
        break;
        
      case 2: // chase
        if (++chaseDelayCount < CHASE_DELAY) return false;    
        chaseDelayCount = 0;
        chaseStart = (chaseStart + 1) % CHASE_GAP;
        
        fill_solid(leds, NUM_LEDS, CRGB::Black);

        for (int8_t i=chaseStart; i<NUM_CHASE; i+=CHASE_GAP) {
          groups[GROUP_SPIRAL].setColour(i, palettes[curPalette].colours[curPaletteIndex]);
          if (cycleIndex) {
            curPaletteIndex = (curPaletteIndex + 1) % palettes[curPalette].size;
          }
        }
        break;
    }
    
    return true;
  }
  
  void changePalette(void) {
    curPalette = random8(0, NUM_PALETTES - 1);
    curPaletteIndex = random8(0, palettes[curPalette].size - 1);
    cycleIndex = (random8() < 180);
  }
  
  void init(void) {
    switch (curSeq) {
      case 0: //twinkle
        for (int8_t i=0; i<NUM_LEDS; i++) {
          bulbs[i].init(15+i, curPaletteIndex, i);
        }
        break;
  
      case 1: // fade
        for (int8_t i=0; i<NUM_LEDS; i++) {
          bulbs[i].init(2, curPaletteIndex, i);
        }
        break;      
    }    
  }
};

#endif

