#include "Groups.h"

Group fairgroundGroups[NUM_FAIRGROUND_GROUPS] = {
  // arms with overlapping centre
  { 5, {{4, 5}, {6, 7}, {8, 9}, {10, 11}, {2, 3}}, {1} },

  // rings
  { 3, {{1}, {2, 4, 6, 8, 10}, {3, 5, 7, 9 ,11}} },
  
  // rows
  { 7, {{7, 9}, {6, 8}, {1}, {4, 10}, {5, 11}, {2}, {3}} },
  
  // spiral
  { 11, {{1}, {4}, {6}, {8}, {10}, {2}, {5}, {7}, {9}, {11}, {3}} }
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

