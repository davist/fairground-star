#ifndef __INC_MOOD_FAIRGROUND_H
#define __INC_MOOD_FAIRGROUND_H

#include "Mood.h"
#include "Palettes.h"
#include "Groups.h"
#include "config.h"

#define SLOW 9
#define MEDIUM 6
#define FAST 3

#define ANIM_PaintAndWipe 1
#define ANIM_Fifo 2
#define ANIM_ContinuousPaint 3
#define ANIM_Marching 4
#define ANIM_Flash 5
#define ANIM_ContinuousPaintMono 6


class MoodFairground : public Mood {
  
  uint8_t frameDelay;
  uint8_t frameDelayCount;

  uint8_t curGroup;
  uint8_t curGroupSize;

  uint8_t curPalette;
  uint8_t curPaletteIndex;
  
  uint8_t seqStep;
  
  bool usedPaintAndWipe;
  bool usedFifo;
  bool usedContinuousPaint;
  bool usedContinuousPaintMono;
  bool usedMarching;
  bool usedFlash;  
  
  uint8_t anim;
  uint8_t animRepeats;
  
public:
  MoodFairground() {
    frameDelay = SLOW;
    frameDelayCount = frameDelay;
    curPaletteIndex = 0;
    changeStep(true);
  }
  
  bool run(void) {

    // wait for frameDelay frames before executing next step
    if (++frameDelayCount < frameDelay) return false;
    
    frameDelayCount = 0;
    
    // run anim step
    bool finished = false;
    switch (anim) {
      case ANIM_PaintAndWipe:
        finished = paintAndWipe();
        break;

      case ANIM_Fifo:
        finished = fifo();
        break;      

      case ANIM_ContinuousPaint:
        finished = continuousPaint();
        break;
      
      case ANIM_ContinuousPaintMono:
        finished = continuousPaintMono();
        break;
      
      case ANIM_Marching:
        finished = marching();
        break;
      
      case ANIM_Flash:
        finished = flash();
        break;
    }
    
    if (finished) {
      animRepeats--;
      if (animRepeats == 0) changeStep(false);    
    }
    
    return true;
  }


private:

  void changePaletteAndGroup(void) {
    curGroup = random8(NUM_FAIRGROUND_GROUPS);
    curGroupSize = fairgroundGroups[curGroup].size;
    curPalette = random8(NUM_PALETTES);
    curPaletteIndex = 0;
    
    // common bulbs have constant colour
    fairgroundGroups[curGroup].setCommon(
      palettes[curPalette].colours[ random8(palettes[curPalette].size) ]
    );
  }

  void changeStep(bool reset) {
    
    // choose next step
    if (reset) {
      seqStep = 0;
    }
    else {
      uint8_t ran8 = random8();
      switch (seqStep) {
      case 0: // initial 1st half anim
        if (ran8 < 150) seqStep = 1;
        else if (ran8 < 220) seqStep = 3;
        else seqStep = 4;
        break;
      case 1: // optional extra 1st half anim
        if (ran8 < 50) seqStep = 2;
        else if (ran8 < 140) seqStep = 3;
        else seqStep = 4;
        break;
      case 2: // optional extra 1st half anim
        if (ran8 < 200) seqStep = 3;
        else seqStep = 4;
        break;
      case 3: // optional flash
        seqStep = 4;
        break;
      case 4: // 2nd half anim
        if (usedFlash) {
          if (ran8 < 200) seqStep = 5;
          else seqStep = 0;
        }
        else {
          if (ran8 < 150) seqStep = 5;
          else if (ran8 < 220) seqStep = 6;
          else seqStep = 0;
        }
        break;
      case 5: // optional extra 2nd half anim
        if (usedFlash) seqStep = 0;
        else {
          if (ran8 < 220) seqStep = 6;
          else seqStep = 0;
        }
        break;
      case 6: // optional flash (if 1st flash didn't happen)
        seqStep = 0;
        break;
      }      
    }
    
    // initialise step
    switch (seqStep) {
      case 0: // initial 1st half anim
        Serial.println("step 0");
        // reset everything
        frameDelay = SLOW;
        usedPaintAndWipe = false;
        usedFifo = false;
        usedContinuousPaint = false;
        usedContinuousPaintMono = false;
        usedMarching = false;
        usedFlash = false;  
        changePaletteAndGroup();
        
        switch (random8(0, 2)) {
          case 0:
            Serial.println("paint/wipe");
            anim = ANIM_PaintAndWipe;
            usedPaintAndWipe = true;
            break;
          case 1:
            Serial.println("fifo");
            anim = ANIM_Fifo;
            usedFifo = true;
            break;
        }
        
        animRepeats = random8(2,4);
        break;
        
      case 1: // optional extra 1st half anim
        Serial.println("step 1");
        frameDelay = MEDIUM;
        switch (random8(0, 2)) {
          case 0:
            Serial.println("paint/wipe");
            anim = ANIM_PaintAndWipe;
            usedPaintAndWipe = true;
            break;
          case 1:
            Serial.println("fifo");
            anim = ANIM_Fifo;
            usedFifo = true;
            break;
        }
        animRepeats = random8(2,4);
        break;

      case 2: // if one anim unused, use it
        Serial.println("step 2");
        
        if (!usedPaintAndWipe)       anim = ANIM_PaintAndWipe;
        else if (!usedFifo)          anim = ANIM_Fifo;
        else if (random8(0, 2) == 0) anim = ANIM_PaintAndWipe;
        else                         anim = ANIM_Fifo;
        
        animRepeats = random8(2,4);
        break;
      
      case 3: // optional flash
        Serial.println("step 3");
        frameDelay = FAST;
        anim = ANIM_Flash;
        animRepeats = random8(4,7);        
        break;
      
      case 4: // 2nd half anim
      case 5:
        if (seqStep == 4) {
          Serial.println("step 4");
          frameDelay = random8(0,2) ? SLOW : MEDIUM;
        }
        else  {
          Serial.println("step 5");
          // speed up in step 5
          if (frameDelay == SLOW) frameDelay = MEDIUM;
          else frameDelay = FAST;
        }
        
        switch (random8(0, 3)) {
          case 0:
            anim = ANIM_ContinuousPaint;
            if (usedContinuousPaint) frameDelay = FAST; // if used before, speed up
            usedContinuousPaint = true;
            animRepeats = random8(3,5);
            break;
          case 1:
            anim = ANIM_ContinuousPaintMono;
            if (usedContinuousPaintMono) frameDelay = FAST; // if used before, speed up
            usedContinuousPaintMono = true;
            animRepeats = random8(6,8);
            break;
          case 2:
            anim = ANIM_Marching;
            if (usedMarching) frameDelay = FAST;
            usedMarching = true;
            animRepeats = random8(3,5);
            break;
        }
        
        break;
      
      case 6: // optional flash (if 1st flash didn't happen)
        Serial.println("step 6");
        frameDelay = FAST;
        anim = ANIM_Flash;
        animRepeats = random8(4,7);        
        break;
    }
  }
  
  bool paintAndWipe(void) {
    static bool painting = true;
    static uint8_t curSetNum = 0;
    
    if (painting) {
      // paint
      fairgroundGroups[curGroup].setColour(curSetNum, palettes[curPalette].colours[curPaletteIndex]);
  
      curPaletteIndex = (curPaletteIndex + 1) % palettes[curPalette].size;
      curSetNum++;
      if (curSetNum == curGroupSize) {
        painting = false;
        curSetNum = 0;
      }
    }
    else {
      // wipe
      fairgroundGroups[curGroup].setColour(curSetNum, CRGB::Black);
  
      curSetNum++;
      if (curSetNum == curGroupSize) {
        painting = true;
        curSetNum = 0;
        
        //finished
        return true;
      }
    }
    
    return false;
  }
  
  bool fifo(void) {
    static bool started = false;
    static bool filling;
    static uint8_t curSetNum;
    static int8_t endSetNum;
    static uint8_t initialPaletteIndex;
    
    if (!started) {
      started = true;
      filling = true;
      curSetNum = 0;
      endSetNum = curGroupSize;
    }
    
    if (filling) {
      // fill      
      if (curSetNum == 0) initialPaletteIndex = curPaletteIndex;
      
      if (curSetNum > 0) {
        fairgroundGroups[curGroup].setColour(curSetNum - 1, CRGB::Black);
      }

      fairgroundGroups[curGroup].setColour(curSetNum, palettes[curPalette].colours[curPaletteIndex]);
  
      curSetNum++;

      if (curSetNum == endSetNum) {
        curSetNum = 0;
        endSetNum--;
        curPaletteIndex = (curPaletteIndex + 1) % palettes[curPalette].size;
        
        if (endSetNum == 0) {
          filling = false;
          endSetNum = curGroupSize - 1;
          curSetNum = endSetNum + 1;
          curPaletteIndex = initialPaletteIndex;
        }
      }
    }
    else {
      // empty
      if (curSetNum > 0) {
        fairgroundGroups[curGroup].setColour(curSetNum - 1, CRGB::Black);
      }

      if (curSetNum < curGroupSize) {
        fairgroundGroups[curGroup].setColour(curSetNum, palettes[curPalette].colours[curPaletteIndex]);
      }
      
      curSetNum++;

      if (curSetNum > curGroupSize) {
        endSetNum--;
        curSetNum = endSetNum + 1;
        curPaletteIndex = (curPaletteIndex + 1) % palettes[curPalette].size;
        
        if (endSetNum < 0) {
          // finished
          started = false;
          return true;
        }
      }
    }
    
    return false;
  }

  bool continuousPaint(void) {
    static uint8_t curSetNum = 0;

    fairgroundGroups[curGroup].setColour(curSetNum, palettes[curPalette].colours[curPaletteIndex]);

    curSetNum++;
    curPaletteIndex = (curPaletteIndex + 1) % palettes[curPalette].size;
    
    if (curSetNum == curGroupSize) {
      curSetNum = 0;
      // finished
      return true;
    }
    
    return false;
  }
  
  bool continuousPaintMono(void) {
    static uint8_t curSetNum = 0;

    fairgroundGroups[curGroup].setColour(curSetNum, palettes[curPalette].colours[curPaletteIndex]);

    curSetNum++;
    
    if (curSetNum == curGroupSize) {
      curSetNum = 0;
      curPaletteIndex = (curPaletteIndex + 1) % palettes[curPalette].size;
      // finished
      return true;
    }
    
    return false;
  }
  
  bool marching(void) {
    static uint8_t offset = 0;
    static uint8_t iterations = 5;

    // check offset isn't out of range due to palette changing since last execution
    if (offset >= palettes[curPalette].size) {
      offset = 0;
    }

    curPaletteIndex = offset;

    for (uint8_t curSetNum=0; curSetNum < curGroupSize; curSetNum++) {
      fairgroundGroups[curGroup].setColour(curSetNum, palettes[curPalette].colours[curPaletteIndex]);
      curPaletteIndex = (curPaletteIndex + 1) % palettes[curPalette].size;
    }
    
    if (offset == 0) offset = palettes[curPalette].size - 1;
    else offset --;
    
    iterations --;
    
    if (iterations == 0) {
      iterations = 5;
      // finished
      return true;
    }
    
    return false;
  }
  
  bool flash(void) {
    static bool flashOn = true;
    
    if (flashOn) {
      for (uint8_t curSetNum=0; curSetNum < curGroupSize; curSetNum++) {
        CRGB col = palettes[curPalette].colours[curPaletteIndex];
        fairgroundGroups[curGroup].setColour(curSetNum, col);
        fairgroundGroups[curGroup].setCommon(col);
        curPaletteIndex = (curPaletteIndex + 1) % palettes[curPalette].size;
      }
      
      flashOn = false;
    }
    else {
      for (uint8_t curSetNum=0; curSetNum < curGroupSize; curSetNum++) {
        fairgroundGroups[curGroup].setColour(curSetNum, CRGB::Black);
        fairgroundGroups[curGroup].setCommon(CRGB::Black);
      }

      flashOn = true;
      
      // finished
      return true;
    }
    
    return false;
  }
  
};

#endif

