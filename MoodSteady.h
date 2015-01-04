#ifndef __INC_MOOD_STEADY_H
#define __INC_MOOD_STEADY_H

#include "Mood.h"
#include "Palettes.h"
#include "Groups.h"

#define NUM_LEDS 11

#define INDEX_DURATION 30000
#define PALETTE_DURATION 300000

#define CHANGE_FRAME_DELAY 5


class MoodSteady : public Mood {
  
  uint8_t curPalette;
  uint8_t curPaletteIndex;
  
  uint32_t lastIndexChangeTime;
  uint32_t lastPaletteChangeTime;
  
  bool changing;
  uint8_t changeGroup;
  uint8_t changeGroupIndex;
  uint8_t frameDelayCount;

public:
  MoodSteady() {
    lastPaletteChangeTime = millis() - PALETTE_DURATION - 1;
    changing = false;
  }
  
  // slower rate of change than default
  uint8_t fadeStep() { return 10; }
  
  bool run(void) {
    
    // time to change palette?
    if (millis() - lastPaletteChangeTime > PALETTE_DURATION) {
      lastPaletteChangeTime = lastIndexChangeTime = millis();
      curPalette = (curPalette + 1) % NUM_PALETTES;
      // start on random index within palette
      curPaletteIndex = random8(0, palettes[curPalette].size - 1);

      startChange();
    }
    else if (millis() - lastIndexChangeTime > INDEX_DURATION ) {
      lastIndexChangeTime = millis();
      curPaletteIndex = (curPaletteIndex + 1) % palettes[curPalette].size;

      startChange();
    }

    if (changing) {
      changing = doChangeStep();
      return true;
    }
    else {
      return false;
    }
  }
  
private:

void startChange(void) {
  changing = true;
  frameDelayCount = CHANGE_FRAME_DELAY;
  changeGroup = random8(NUM_STEADY_GROUPS);
  changeGroupIndex = 0;
}

bool doChangeStep(void) {
  // wait for frameDelay frames before executing next step
  if (++frameDelayCount < CHANGE_FRAME_DELAY) return true;

  frameDelayCount = 0;

  steadyGroups[changeGroup].setColour(changeGroupIndex, palettes[curPalette].colours[curPaletteIndex]);

  changeGroupIndex++;
  
  if (changeGroupIndex == steadyGroups[changeGroup].size) {
    // finished
    return false;
  }
  else {
    // keep changing
    return true;
  }
}

  
};

#endif

