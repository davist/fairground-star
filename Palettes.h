#ifndef __INC_PALETTES_H
#define __INC_PALETTES_H

#include "config.h"

#define NUM_PALETTES 8
#define NUM_FAIRGROUND_PALETTES 6
#define NUM_TWINKLE_PALETTES 6

struct Palette {
  uint8_t size;
  CRGB colours[8];
};

extern const Palette palettes[NUM_PALETTES];
extern const uint8_t fairgroundPalettes[NUM_FAIRGROUND_PALETTES];
extern const uint8_t twinklePalettes[NUM_TWINKLE_PALETTES];



#endif

