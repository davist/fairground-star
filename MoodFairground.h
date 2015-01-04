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
    
    // common bulbs have constant colour
    fairgroundGroups[curGroup].setCommon(
      palettes[curPalette].colours[ random8(palettes[curPalette].size) ]
    );
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
        if (configFlash.used) seqStep = 0;
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
        changePaletteAndGroup();
        initAnimConfigs();
        
        switch (random8(0, 2)) {
          case 0:
            Serial.println("paint/wipe");
            anim = ANIM_PaintAndWipe;
            configPaintAndWipe.used = true;
            break;
          case 1:
            Serial.println("fifo");
            anim = ANIM_Fifo;
            configFifo.used = true;
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
            configPaintAndWipe.used = true;
            break;
          case 1:
            Serial.println("fifo");
            anim = ANIM_Fifo;
            configFifo.used = true;
            break;
        }
        animRepeats = random8(2,4);
        break;

      case 2: // if one anim unused, use it
        Serial.println("step 2");
        
        if (!configPaintAndWipe.used) anim = ANIM_PaintAndWipe;
        else if (!configFifo.used)    anim = ANIM_Fifo;
        else if (random8() < 128)     anim = ANIM_PaintAndWipe;
        else                          anim = ANIM_Fifo;
        
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
            if (configContinuousPaint.used) frameDelay = FAST; // if used before, speed up
            configContinuousPaint.used = true;
            animRepeats = random8(3,5);
            break;
          case 1:
            anim = ANIM_ContinuousPaintMono;
            if (configContinuousPaintMono.used) frameDelay = FAST; // if used before, speed up
            configContinuousPaintMono.used = true;
            animRepeats = random8(6,8);
            break;
          case 2:
            anim = ANIM_Marching;
            configMarching.used = true;
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
      if (configFifo.curSetNum == 0) configFifo.initialPaletteIndex = curPaletteIndex;
      
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
  
};

#endif

