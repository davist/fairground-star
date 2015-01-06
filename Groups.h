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

#ifndef __INC_GROUPS_H
#define __INC_GROUPS_H

#include "config.h"

#define NUM_FAIRGROUND_GROUPS 4
#define NUM_TWINKLE_GROUPS 2
#define NUM_STEADY_GROUPS 5

#define GROUP_FAIRGROUND_ARMS 0
#define GROUP_FAIRGROUND_RINGS 1
#define GROUP_FAIRGROUND_ROWS 2
#define GROUP_FAIRGROUND_SPIRAL 3
#define GROUP_FAIRGROUND_ARMS2 4
#define GROUP_FAIRGROUND_ROWS_REV 5

#define GROUP_TWINKLE_INDIVIDUAL 0
#define GROUP_TWINKLE_ARMS 1

struct Group {
  // number of sets in this group
  uint8_t size;
  
  // defines the leds in each set. Max 11 sets with 5 leds each.
  // led numbers start at 1, not 0 so zero can be used to indicate undefined
  uint8_t sets[11][5];
  
  // option common led - not part of any set
  uint8_t common[1];
  
  /**
   * Sets all leds in the given set to the given colour
   * setNum: 0 to group.size - 1
   * colour: a CRGB colour specification
   */
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

  /**
   * Sets the colour of the common led
   */  
  void setCommon(CRGB colour) {
    if (common[0] > 0) {
      leds[common[0] - 1] = colour;
    }
  }
};

extern Group fairgroundGroups[NUM_FAIRGROUND_GROUPS + 2];
extern Group twinkleGroups[NUM_TWINKLE_GROUPS];
extern Group steadyGroups[NUM_STEADY_GROUPS];

#endif

