#include <stdint.h>
#include "Arduino.h"
#include "color.h"

#ifndef __RGB_LED_H
#define __RGB_LED_H

class RgbLed;

class RgbLedAnimationStep{
protected:
public:
  unsigned int _duration;
  unsigned int _start;
  RgbLedAnimationStep(unsigned int start);
  virtual boolean isActive(RgbLed* animation);
//  virtual boolean isEnded() = 0;
  virtual void run(RgbLed* animation) = 0;
  void setDuration(unsigned int duration);
  unsigned int getStart();
};

class RLAFadeTo : public RgbLedAnimationStep
{
  private:
  boolean _initialized;
  Color _sourceColor;
  Color _newColor;
  public:
  RLAFadeTo(unsigned int start, Color newColor);
  void run(RgbLed* animation);
};

class RLASetTo : public RgbLedAnimationStep
{
  private:
  Color _newColor;
  public:
  RLASetTo(unsigned int start, Color newColor);
  void run(RgbLed* animation);
};

class RLAWait: public RgbLedAnimationStep
{
  public:
  RLAWait(unsigned int start);
  void run(RgbLed* animation);
};

class RLALoopTo: public RgbLedAnimationStep
{
private:
  unsigned int _restartAt;
  int _occurences;  
public:
  RLALoopTo(unsigned int start, unsigned int restartAt, int occurences);
  boolean isActive(RgbLed* animation);
  void run(RgbLed* animation);
};

class RLAStop: public RgbLedAnimationStep
{
  public:
  RLAStop(unsigned int start);
  boolean isActive(RgbLed* animation);
  void run(RgbLed* animation);
};

class RgbLed
{
private:
  RgbLedAnimationStep** _steps;
  byte _stepsNb;
  boolean _isActive;
  byte _pinR;
  byte _pinG;
  byte _pinB;
  unsigned long _previousMillis;
  float _brightness;
  Color _calibration;
public:
  Color _currentColor;
  unsigned long _currentTime;
  void setColor(Color color);
  friend class RgbLedAnimationStep;

  RgbLed();
  ~RgbLed();
  void init(byte pinR, byte pinG, byte pinB);
  void runAnimation();
  void startAnimation();
  void stopAnimation();
  void resumeAnimation();
  void setAnimation(int arraySize, RgbLedAnimationStep** steps);
  void resetAnimation();
  void setBrightness(float b);
  float getBrightness();
  boolean isActive();
  void setCalibration(Color c);
};

#endif
