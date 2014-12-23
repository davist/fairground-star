#include "Groups.h"

Group groups[NUM_GROUPS] = {
  // arms with overlapping centre
  { 5, {{2, 3}, {4, 5}, {6, 7}, {8, 9}, {10, 11}}, {1} },

  // rings
  { 3, {{1}, {2, 4, 6, 8, 10}, {3, 5, 7, 9 ,11}} },
  
  // rows
  { 7, {{7, 9}, {6, 8}, {1}, {4, 10}, {5, 11}, {2}, {3}} },
  
  // spiral
  { 11, {{1}, {4}, {6}, {8}, {10}, {2}, {5}, {7}, {9}, {11}, {3}} },

  // arms with separate centre
  { 6, {{1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}, {10, 11}} } 
};
