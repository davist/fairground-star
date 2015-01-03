#ifndef __INC_MOOD_STEADY_H
#define __INC_MOOD_STEADY_H

#include "Mood.h"
#include "Palettes.h"
#include "Groups.h"

#define NUM_LEDS 11

#define INDEX_DURATION 30000
#define PALETTE_DURATION 300000


class MoodSteady : public Mood {
  
  uint8_t curPalette;
  uint8_t curPaletteIndex;
  
  uint32_t lastIndexChangeTime;
  uint32_t lastPaletteChangeTime;
  
public:
  MoodSteady() {
    lastPaletteChangeTime = millis() - PALETTE_DURATION - 1;
  }
  
  bool run(void) {
    // time to change palette?
    if (millis() - lastPaletteChangeTime > PALETTE_DURATION) {
      lastPaletteChangeTime = lastIndexChangeTime = millis();
      curPalette = (curPalette + 1) % NUM_PALETTES;
      // start on random index within palette
      curPaletteIndex = random8(0, palettes[curPalette].size - 1);

      fill_solid(leds, NUM_LEDS, palettes[curPalette].colours[curPaletteIndex]);
      return true;
    }
    else if (millis() - lastIndexChangeTime > INDEX_DURATION ) {
      lastIndexChangeTime = millis();
      curPaletteIndex = (curPaletteIndex + 1) % palettes[curPalette].size;

      fill_solid(leds, NUM_LEDS, palettes[curPalette].colours[curPaletteIndex]);
      return true;
    }

    return false;
  }
};

#endif

