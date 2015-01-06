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

#ifndef __INC_CONFIG_H
#define __INC_CONFIG_H

#include <FastLED.h>

// led strip
#define LED_TYPE WS2812B
#define RGB_ORDER GRB
#define NUM_LEDS 11
#define DATA_PIN 12

// animation speed in frames per second
#define FPS 30
// time per frame in ms
#define FRAME_TIME (1000/(FPS))

// touch sensor
#define TOUCH_PIN 2
#define TOUCH_INT 0

#define LONG_TOUCH_TIME 1000


// Define the array of leds
extern CRGB leds[NUM_LEDS];

#endif

