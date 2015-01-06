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

#include "Groups.h"

Group fairgroundGroups[NUM_FAIRGROUND_GROUPS + 2] = {
  // arms with common centre
  { 5, {{4, 5}, {6, 7}, {8, 9}, {10, 11}, {2, 3}}, {1} },

  // rings
  { 3, {{1}, {2, 4, 6, 8, 10}, {3, 5, 7, 9 ,11}} },
  
  // rows
  { 7, {{7, 9}, {6, 8}, {1}, {4, 10}, {5, 11}, {2}, {3}} },
  
  // spiral
  { 11, {{1}, {4}, {6}, {8}, {10}, {2}, {5}, {7}, {9}, {11}, {3}} },

  // arms with common centre - mid set at top
  { 5, {{8, 9}, {10, 11}, {2, 3}, {4, 5}, {6, 7}}, {1} },

  // rows reversed
  { 7, {{3}, {2}, {5, 11}, {4, 10}, {1}, {6, 8}, {7, 9}} }
};

Group twinkleGroups[NUM_TWINKLE_GROUPS] = {
  // individual
  { 11, {{1}, {7}, {10}, {4}, {3}, {8}, {5}, {6}, {11}, {2}, {9}} },
  // arms with separate centre
  { 6, {{1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}, {10, 11}} } 
};

Group steadyGroups[NUM_STEADY_GROUPS] = {
  // wave from top
  { 6, {{3}, {2}, {4, 10}, {1, 5, 11}, {6, 8}, {7, 9}} },
  // wave from top-left
  { 6, {{11}, {10}, {2, 8}, {1, 3, 9}, {4, 6}, {5, 7}} },
  // wave from top-right
  { 6, {{5}, {4}, {2, 6}, {1, 3, 7}, {8, 10}, {9, 11}} },
  // wave from bottom-left
  { 6, {{9}, {8}, {10, 6}, {1, 7, 11}, {2, 4}, {3, 5}} },
  // wave from bottom-right
  { 6, {{7}, {6}, {4, 8}, {1, 5, 9}, {2, 10}, {3, 11}} },
};

