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

