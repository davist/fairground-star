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

#ifndef __INC_MOOD_FAIRGROUND_H
#define __INC_MOOD_FAIRGROUND_H

#include "Mood.h"
#include "Palettes.h"
#include "Groups.h"
#include "config.h"

#define SLOW 9
#define MEDIUM 6
#define FAST 3

#define NUM_ANIMS 7

#define ANIM_PaintAndWipe 0
#define ANIM_Fifo 1
#define ANIM_ContinuousPaint 2
#define ANIM_Marching 3
#define ANIM_Flash 4
#define ANIM_ContinuousPaintMono 5
#define ANIM_CentrePaintAndWipeMono 6


struct AnimGroupConfig {
  uint8_t repeats;  
  uint8_t lowSpeed;
  uint8_t hiSpeed;
};

// sensible repetion and speed settings for each combination of group and anim
const AnimGroupConfig animGroupConfig[NUM_FAIRGROUND_GROUPS][NUM_ANIMS] = {
  // arms
  {
    {3, SLOW, MEDIUM}, // paint and wipe
    {2, SLOW, MEDIUM}, // fifo
    {4, MEDIUM, MEDIUM}, // cont. paint
    {2, SLOW, MEDIUM}, // marching
    {7, FAST, FAST}, // flash
    {5, MEDIUM, FAST}, // cont. paint mono
    {5, SLOW, MEDIUM} // centre paint and wipe mono
  },
  
  // rings
  {
    {4, SLOW, MEDIUM}, // paint and wipe
    {3, SLOW, MEDIUM}, // fifo
    {5, SLOW, MEDIUM}, // cont. paint
    {2, SLOW, MEDIUM}, // marching
    {7, FAST, FAST}, // flash
    {5, SLOW, MEDIUM}, // cont. paint mono
    {5, SLOW, MEDIUM} // centre paint and wipe mono
  },
  
  // rows
  {
    {3, SLOW, MEDIUM}, // paint and wipe
    {2, SLOW, MEDIUM}, // fifo
    {5, MEDIUM, MEDIUM}, // cont. paint
    {3, SLOW, SLOW}, // marching
    {7, FAST, FAST}, // flash
    {7, MEDIUM, FAST}, // cont. paint mono
    {5, MEDIUM, FAST} // centre paint and wipe mono
  },
  
  // spiral
  {
    {2, SLOW, MEDIUM}, // paint and wipe
    {1, SLOW, MEDIUM}, // fifo
    {4, MEDIUM, MEDIUM}, // cont. paint
    {3, SLOW, SLOW}, // marching
    {7, FAST, FAST}, // flash
    {8, MEDIUM, FAST}, // cont. paint mono
    {0, 0, 0} // centre paint and wipe mono
  }
  
};

class MoodFairground : public Mood {
  
  uint8_t frameDelay;
  uint8_t frameDelayCount;

  uint8_t curGroup;
  uint8_t curGroupSize;

  uint8_t curPalette;
  uint8_t curPaletteIndex;
  
  uint8_t seqStep;
  
  uint8_t anim;
  uint8_t animRepeats;
  
  struct {
    bool painting;
    uint8_t curSetNum;
    bool used;
  } configPaintAndWipe;
  
  struct {
    bool started;
    bool filling;
    uint8_t curSetNum;
    int8_t endSetNum;
    uint8_t initialPaletteIndex;
    bool used;
  } configFifo;
  
  struct {
    uint8_t curSetNum;
    bool used;
  } configContinuousPaint;
  
  struct {
    uint8_t curSetNum;
    bool used;
  } configContinuousPaintMono;
  
  struct {
    uint8_t offset;
    uint8_t iterations;
    bool used;
  } configMarching;
  
  struct {
    bool flashOn;
    bool used;
  } configFlash;
  
  struct {
    bool painting;
    uint8_t offset;
    bool used;
  } configCentrePaintAndWipeMono;
  
  
public:

  /////////////////////////////////////////

  MoodFairground() {
    frameDelay = SLOW;
    frameDelayCount = frameDelay;
    curPaletteIndex = 0;
    changeStep(true);
  }
  
  /////////////////////////////////////////

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

      case ANIM_CentrePaintAndWipeMono:
        finished = centrePaintAndWipeMono();
        break;
    }
    
    if (finished) {
      animRepeats--;
      if (animRepeats == 0) changeStep(false);    
    }
    
    return true;
  }


private:

  /////////////////////////////////////////

  void changePaletteAndGroup(void) {
    curGroup = random8(NUM_FAIRGROUND_GROUPS);
    curGroupSize = fairgroundGroups[curGroup].size;
    curPalette = fairgroundPalettes[random8(NUM_FAIRGROUND_PALETTES)];
    curPaletteIndex = 0;    
  }

  /////////////////////////////////////////

  void initAnimConfigs(void) {
    configPaintAndWipe.painting = true;
    configPaintAndWipe.curSetNum = 0;
    configPaintAndWipe.used = false;

    configFifo.filling = true;
    configFifo.curSetNum = 0;
    configFifo.endSetNum = curGroupSize;
    configFifo.initialPaletteIndex;
    configFifo.used = false;
  
    configContinuousPaint.curSetNum = 0;
    configContinuousPaint.used = false;
  
    configContinuousPaintMono.curSetNum = 0;
    configContinuousPaintMono.used = false;
  
    configMarching.offset = 0;
    configMarching.iterations = 5;
    configMarching.used = false;
  
    configFlash.flashOn = true;
    configFlash.used = false;

    configCentrePaintAndWipeMono.offset = 0;
    configCentrePaintAndWipeMono.used = false;  
  };
  
  /////////////////////////////////////////

  void changeStep(bool reset) {
    
    // choose next step
    if (reset) {
      seqStep = 0;
    }
    else {
      uint8_t ran8 = random8();
      switch (seqStep) {
      case 0: // initial 1st half anim
        if (ran8 < 170) seqStep = 1;
        else if (ran8 < 200) seqStep = 3;
        else seqStep = 4;
        break;
      case 1: // optional extra 1st half anim
        if (ran8 < 80) seqStep = 2;
        else if (ran8 < 120) seqStep = 3;
        else seqStep = 4;
        break;
      case 2: // optional extra 1st half anim
        if (ran8 < 100) seqStep = 3;
        else seqStep = 4;
        break;
      case 3: // optional flash
        seqStep = 4;
        break;
      case 4: // 2nd half anim
        if (configFlash.used) {
          if (ran8 < 200) seqStep = 5;
          else seqStep = 0;
        }
        else {
          if (ran8 < 150) seqStep = 5;
          else if (ran8 < 190) seqStep = 6;
          else seqStep = 0;
        }
        break;
      case 5: // optional extra 2nd half anim
        if (configFlash.used) seqStep = 0;
        else {
          if (ran8 < 100) seqStep = 6;
          else seqStep = 0;
        }
        break;
      case 6: // optional flash (if 1st flash didn't happen)
        seqStep = 0;
        break;
      }      
    }
    
    do {
      initialiseStep();
    } while (animRepeats == 0);
    
    // common bulbs have constant colour
    fairgroundGroups[curGroup].setCommon(
      palettes[curPalette].colours[ random8(palettes[curPalette].size) ]
    );
  }
  
  void initialiseStep(void) {

    if (curGroup == GROUP_FAIRGROUND_ARMS2) {
      // reset override
      curGroup = GROUP_FAIRGROUND_ARMS;
    }
    else if (curGroup == GROUP_FAIRGROUND_ROWS_REV) {
      // reset override
      curGroup = GROUP_FAIRGROUND_ROWS;
    }

    switch (seqStep) {
      case 0: // initial 1st half anim
        // reset everything
        changePaletteAndGroup();
        initAnimConfigs();
        
        // fall thru
        
      case 1: // optional extra 1st half anim
        switch (random8(0, 3)) {
          case 0:
            anim = ANIM_PaintAndWipe;
            configPaintAndWipe.used = true;
            break;
          case 1:
            anim = ANIM_Fifo;
            configFifo.used = true;
            break;
          case 2:
            anim = ANIM_CentrePaintAndWipeMono;
            configCentrePaintAndWipeMono.used = true;
            break;
        }

        if (seqStep == 0) {
          frameDelay = animGroupConfig[curGroup][anim].lowSpeed;
        }
        else  {
          // speed up in step 1
          frameDelay = animGroupConfig[curGroup][anim].hiSpeed;
        }
        break;

      case 2: // if one anim unused, use it
        
        if (animGroupConfig[curGroup][anim].repeats == 0) {
          // appear to have previously chosen a disabled anim and as this
          // step is deterministic, no point re-running it
          switch (random8(0, 3)) {
            case 0:
              anim = ANIM_PaintAndWipe;
              break;
            case 1:
              anim = ANIM_Fifo;
              break;
            case 2:
              anim = ANIM_CentrePaintAndWipeMono;
              break;
          }          
        }
        else {
          if (!configPaintAndWipe.used) anim = ANIM_PaintAndWipe;
          else if (!configFifo.used)    anim = ANIM_Fifo;
          else if (!configCentrePaintAndWipeMono.used)    anim = ANIM_CentrePaintAndWipeMono;
          else                          anim = ANIM_Fifo;
        }
        
        frameDelay = animGroupConfig[curGroup][anim].hiSpeed;
        break;
      
      case 3: // optional flash
      case 6:
        anim = ANIM_Flash;
        frameDelay = animGroupConfig[curGroup][anim].hiSpeed;
        break;
      
      case 4: // 2nd half anim
      case 5:
        switch (random8(0, 3)) {
          case 0:
            anim = ANIM_ContinuousPaint;
            configContinuousPaint.used = true;
            break;
          case 1:
            anim = ANIM_ContinuousPaintMono;
            configContinuousPaintMono.used = true;
            break;
          case 2:
            anim = ANIM_Marching;
            configMarching.used = true;
            break;
        }
        
        if (seqStep == 4) {
          frameDelay = animGroupConfig[curGroup][anim].lowSpeed;
        }
        else  {
          // speed up in step 5
          frameDelay = animGroupConfig[curGroup][anim].hiSpeed;
        }
        
        break;      
    }
    
    animRepeats = animGroupConfig[curGroup][anim].repeats;
    
    // group overrides for particular anims
    if (anim == ANIM_CentrePaintAndWipeMono && curGroup == GROUP_FAIRGROUND_ARMS) {
      curGroup = GROUP_FAIRGROUND_ARMS2;
    }    
    else if (anim == ANIM_Fifo && curGroup == GROUP_FAIRGROUND_ROWS) {
      curGroup = GROUP_FAIRGROUND_ROWS_REV;
    }
    
    
  }
  
  /////////////////////////////////////////
  // Animations
  /////////////////////////////////////////

  inline void setPaletteColour(uint8_t setNum) {
    fairgroundGroups[curGroup].setColour(setNum, palettes[curPalette].colours[curPaletteIndex]);
  }
  
  /////////////////////////////////////////

  inline void setBlack(uint8_t setNum) {
    fairgroundGroups[curGroup].setColour(setNum, CRGB::Black);
  }

  /////////////////////////////////////////

  inline void incPaletteIndex(void) {
    curPaletteIndex = (curPaletteIndex + 1) % palettes[curPalette].size;
  }
  
  /////////////////////////////////////////

  bool paintAndWipe(void) {
    
    if (configPaintAndWipe.painting) {
      // paint
      setPaletteColour(configPaintAndWipe.curSetNum);
  
      incPaletteIndex();
      configPaintAndWipe.curSetNum++;
      if (configPaintAndWipe.curSetNum == curGroupSize) {
        configPaintAndWipe.painting = false;
        configPaintAndWipe.curSetNum = 0;
      }
    }
    else {
      // wipe
      setBlack(configPaintAndWipe.curSetNum);
  
      configPaintAndWipe.curSetNum++;
      if (configPaintAndWipe.curSetNum == curGroupSize) {
        configPaintAndWipe.painting = true;
        configPaintAndWipe.curSetNum = 0;
        
        //finished
        return true;
      }
    }
    
    return false;
  }
  
  /////////////////////////////////////////

  bool fifo(void) {
    
    if (configFifo.filling) {
      // fill      
      
      if (configFifo.curSetNum == 0 && configFifo.endSetNum == curGroupSize) {
        // record first colour used when filling so the same first colour
        // can be used for emptying
        configFifo.initialPaletteIndex = curPaletteIndex;
      }
      
      if (configFifo.curSetNum > 0) {
        setBlack(configFifo.curSetNum - 1);
      }

      setPaletteColour(configFifo.curSetNum);
  
      configFifo.curSetNum++;

      if (configFifo.curSetNum == configFifo.endSetNum) {
        configFifo.curSetNum = 0;
        configFifo.endSetNum--;
        incPaletteIndex();
        
        if (configFifo.endSetNum == 0) {
          configFifo.filling = false;
          configFifo.endSetNum = curGroupSize - 1;
          configFifo.curSetNum = configFifo.endSetNum + 1;
          curPaletteIndex = configFifo.initialPaletteIndex;
        }
      }
    }
    else {
      // empty
      if (configFifo.curSetNum > 0) {
        setBlack(configFifo.curSetNum - 1);
      }

      if (configFifo.curSetNum < curGroupSize) {
        setPaletteColour(configFifo.curSetNum);
      }
      
      configFifo.curSetNum++;

      if (configFifo.curSetNum > curGroupSize) {
        configFifo.endSetNum--;
        configFifo.curSetNum = configFifo.endSetNum + 1;
        incPaletteIndex();
        
        if (configFifo.endSetNum < 0) {
          // finished
          configFifo.filling = true;
          configFifo.curSetNum == 0;
          configFifo.endSetNum = curGroupSize;
          return true;
        }
      }
    }
    
    return false;
  }

  /////////////////////////////////////////

  bool continuousPaint(void) {

    setPaletteColour(configContinuousPaint.curSetNum);

    configContinuousPaint.curSetNum++;
    incPaletteIndex();
    
    if (configContinuousPaint.curSetNum == curGroupSize) {
      configContinuousPaint.curSetNum = 0;
      // finished
      return true;
    }
    
    return false;
  }
  
  /////////////////////////////////////////

  bool continuousPaintMono(void) {

    setPaletteColour(configContinuousPaintMono.curSetNum);

    configContinuousPaintMono.curSetNum++;
    
    if (configContinuousPaintMono.curSetNum == curGroupSize) {
      configContinuousPaintMono.curSetNum = 0;
      incPaletteIndex();
      // finished
      return true;
    }
    
    return false;
  }
  
  /////////////////////////////////////////

  bool marching(void) {

    // check offset isn't out of range due to palette changing since last execution
    if (configMarching.offset >= palettes[curPalette].size) {
      configMarching.offset = 0;
    }

    curPaletteIndex = configMarching.offset;

    for (uint8_t curSetNum=0; curSetNum < curGroupSize; curSetNum++) {
      setPaletteColour(curSetNum);
      incPaletteIndex();
    }
    
    if (configMarching.offset == 0) configMarching.offset = palettes[curPalette].size - 1;
    else configMarching.offset --;
    
    configMarching.iterations --;
    
    if (configMarching.iterations == 0) {
      configMarching.iterations = 5;
      // finished
      return true;
    }
    
    return false;
  }
  
  /////////////////////////////////////////

  bool flash(void) {
    
    if (configFlash.flashOn) {
      for (uint8_t curSetNum=0; curSetNum < curGroupSize; curSetNum++) {
        setPaletteColour(curSetNum);
        fairgroundGroups[curGroup].setCommon(palettes[curPalette].colours[curPaletteIndex]);
        incPaletteIndex();
      }
      
      configFlash.flashOn = false;
    }
    else {
      for (uint8_t curSetNum=0; curSetNum < curGroupSize; curSetNum++) {
        setBlack(curSetNum);
        fairgroundGroups[curGroup].setCommon(CRGB::Black);
      }

      configFlash.flashOn = true;
      
      // finished
      return true;
    }
    
    return false;
  }
  
  /////////////////////////////////////////

  bool centrePaintAndWipeMono(void) {
    
    uint8_t midPoint = curGroupSize / 2;
    
    fairgroundGroups[curGroup].setCommon(
      palettes[curPalette].colours[curPaletteIndex]
    );

    for (uint8_t set = midPoint - configCentrePaintAndWipeMono.offset;
         set <= midPoint + configCentrePaintAndWipeMono.offset;
         set++) {      

      // paint or wipe
      if (configCentrePaintAndWipeMono.painting) {
        setPaletteColour(set);
      }
      else {
        setBlack(set);
      }
    }
      
    configCentrePaintAndWipeMono.offset++;

    if (midPoint + configCentrePaintAndWipeMono.offset == curGroupSize) {
      // full up, switch mode
      configCentrePaintAndWipeMono.offset = 0;
      configCentrePaintAndWipeMono.painting = !configCentrePaintAndWipeMono.painting;
      
      if (configCentrePaintAndWipeMono.painting) {
        // just changed to painting mode
        incPaletteIndex();        
        //finished
        return true;
      }
    }

    return false;
  }
  
};

#endif

