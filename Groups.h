#ifndef __INC_GROUPS_H
#define __INC_GROUPS_H

#include "FastLED.h"

extern CRGB leds[];

#define NUM_GROUPS 5
#define NUM_FAIRGROUND_GROUPS 4

#define GROUP_ARMS 0
#define GROUP_RINGS 1
#define GROUP_ROWS 2
#define GROUP_SPIRAL 3
#define GROUP_ARMS2 4

struct Group {
  uint8_t size;
  uint8_t sets[11][5];
  uint8_t common[1];
  
  void setColour(uint8_t setNum, CRGB colour) {
    if (setNum < size) {
      for (uint8_t i=0; i<5; i++) {
        if (sets[setNum][i] > 0) {
          leds[sets[setNum][i] - 1] = colour;
        }
        else {
          // end of set
          break;
        }
      }
    }
  }
  
  void setCommon(CRGB colour) {
    if (common[0] > 0) {
      leds[common[0] - 1] = colour;
    }
  }
};

extern Group groups[NUM_GROUPS];

#endif

