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

#include "Palettes.h"

const Palette palettes[NUM_PALETTES] = {

  // red, green
  { 2, CRGB::Red, CRGB::Green },
  
  // warm whites  
  { 4, 0x8c8c80, 0xfff04a, 0xffc73b, 0xffb24d },
  
  // blue, pink, yellow
  { 3, 0x2e7fff, 0xe472bd, 0xf8ea36 },
  
  // rainbow
  { 6, CRGB::Red, CRGB::Yellow, CRGB::Green, CRGB::Cyan, CRGB::Blue, CRGB::Magenta },
  
  // blue, gold
  { 2, 0x1800df, 0xffd815 },
  
  // red, white, blue
  { 3, 0xff3f28, 0xe8e8f8, 0x213bff },
  
  // ocean
  { 5, 0xf6e82a, 0xcfc146, 0x13a69c, 0x00618c, 0x002c83 },
  
  // orange, purple, green
  { 3, 0xff7f00, 0xaa00aa, 0x20e000  }
};

const uint8_t fairgroundPalettes[NUM_FAIRGROUND_PALETTES] = {
  0, 2, 3, 4, 5, 7
};

const uint8_t twinklePalettes[NUM_TWINKLE_PALETTES] = {
  1, 2, 3, 5, 6, 7
};

