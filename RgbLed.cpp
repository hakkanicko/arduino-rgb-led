#include "RgbLed.h"
#include "Logger.h"


/**

 */
RgbLed::RgbLed()
{
  this->_isActive = false;
  this->_stepsNb = 0;
  this->_brightness = 1;
  this->_calibration = Color(255,255,255);
}
RgbLed::~RgbLed()
{
  for(unsigned short i=0; i<this->_stepsNb; i++)
  {
    //DEBUG("[SET] Delete "+String(i));
    delete this->_steps[i];
  }
  delete this->_steps;
}
void RgbLed::setCalibration(Color c)
{
  this->_calibration = c;
}

void RgbLed::setBrightness(float b)
{
  this->_brightness = b;
  if (!this->_isActive)
  {
    this->setColor(this->_currentColor);
  }
}
float RgbLed::getBrightness()
{
  return this->_brightness;
}
void RgbLed::init(byte pinR, byte pinG, byte pinB)
{
  this->_pinR = pinR;
  this->_pinG = pinG;
  this->_pinB = pinB;
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);

}
void RgbLed::setColor(Color color)
{
  //DEBUG("Set color : " + color.toString());
  this->_currentColor = color;
  analogWrite(this->_pinR, (byte)(this->_brightness*(color.getR()*this->_calibration.getR()/255)));
  analogWrite(this->_pinG, (byte)(this->_brightness*(color.getG()*this->_calibration.getG()/255)));
  analogWrite(this->_pinB, (byte)(this->_brightness*(color.getB()*this->_calibration.getB()/255)));
}
void RgbLed::runAnimation()
{
  if (!this->_isActive)
  {
    return;
  }
  this->_currentTime += millis() - this->_previousMillis;
  this->_previousMillis = millis();
  for(unsigned short i=0; i<this->_stepsNb; i++)
  {
    //DEBUG("[RUN] Test for step " + String(i) + " - Millis : " + String(this->_currentTime));
    if (this->_steps[i]->isActive(this))
    {
      //DEBUG("Launch step " + String(i));
      this->_steps[i]->run(this);
      break;
    }
  }
}
void RgbLed::startAnimation()
{
  if (!this->_isActive)
  {
    this->_currentTime=0;
    this->_previousMillis=millis();
    this->_isActive=true;
    //DEBUG("Start Animation");
  }
}
void RgbLed::stopAnimation()
{
  this->_isActive=false;
  //DEBUG("Stop Animation");
}

boolean RgbLed::isActive()
{
  return this->_isActive;
}
void RgbLed::resumeAnimation()
{
  if (!this->_isActive)
  {
    this->_previousMillis=millis();
    this->_isActive=true;
    //DEBUG("Resume Animation");
  }
}
void RgbLed::setAnimation(int arraySize, RgbLedAnimationStep* steps[])
{
  //DEBUG("[SET] Begins.");
  this->resetAnimation();
  //DEBUG("[SET] : malloc : " + String(sizeof(steps)));
  this->_steps=(RgbLedAnimationStep**)malloc(arraySize*sizeof(RgbLedAnimationStep*));
  this->_stepsNb=arraySize;
  for(unsigned short i=0; i<arraySize;i++)
  {
    this->_steps[i] = (steps[i]);
    //DEBUG(" [SET] : Copy from " + String((long)steps[i]) + " to " + String((long)this->_steps[i]));
    //DEBUG("[SET] : ??? " + String(i) + " duration : " + " - " + String((long)this->_steps[i]));
    if(i<(arraySize-1))
    {
       this->_steps[i]->setDuration(steps[i+1]->getStart() - steps[i]->getStart());
       //DEBUG("[SET] : Copy " + String(i) + " duration : " + " - " + String((long)this->_steps[i]));
    }
  }
 
}

void RgbLed::resetAnimation()
{
    for(unsigned short i=0; i<this->_stepsNb; i++)
  {
    //DEBUG("[SET] Delete "+String(i));
    delete this->_steps[i];
  }
  delete this->_steps;
}

RgbLedAnimationStep::RgbLedAnimationStep(unsigned int start)
{
  this->_start = start;
}
boolean RgbLedAnimationStep::isActive(RgbLed* animation)
{
  //DEBUG("[isActive] " + String(this->_start) + " - " + String(this->_duration));
  return (animation->_currentTime >= this->_start) && (animation->_currentTime < (this->_start + this->_duration));
}
void RgbLedAnimationStep::setDuration(unsigned int duration)
{
  this->_duration = duration;
}
unsigned int RgbLedAnimationStep::getStart()
{
  return this->_start;
}

RLAFadeTo::RLAFadeTo(unsigned int start, Color newColor) : RgbLedAnimationStep(start)
{
  this->_newColor = newColor;
  this->_initialized=false;
}
void RLAFadeTo::run(RgbLed* animation)
{
  if (!this->_initialized)
  {
    //DEBUG("[FADE]INIT");
    this->_sourceColor = animation->_currentColor;
    this->_initialized=true;
  }
//  DEBUG(String(this->_sourceColor.getR()) + " + (" + String(this->_newColor.getR()) + " - " + 
  float f=(float)(animation->_currentTime-this->_start)/(float)this->_duration;
  byte r = this->_sourceColor.getR() + (int)((this->_newColor.getR()-this->_sourceColor.getR())*f);
  byte g = this->_sourceColor.getG() + (int)((this->_newColor.getG()-this->_sourceColor.getG())*f);
  byte b = this->_sourceColor.getB() + (int)((this->_newColor.getB()-this->_sourceColor.getB())*f);
    
  Color c = Color(r,g,b);
  animation->setColor(c);
}



RLASetTo::RLASetTo(unsigned int start, Color newColor) : RgbLedAnimationStep(start)
{
  this->_newColor=newColor;
}
void RLASetTo::run(RgbLed* animation)
{
  animation->setColor(this->_newColor);
}

RLALoopTo::RLALoopTo(unsigned int start, unsigned int restartAt, int occurences) : RgbLedAnimationStep(start)
{
  this->_restartAt = restartAt;
  this->_occurences = occurences;
}
boolean RLALoopTo::isActive(RgbLed* animation)
{
  return (animation->_currentTime >= this->_start) && this->_occurences != 0;
}
void RLALoopTo::run(RgbLed* animation)
{
  if (this->_occurences > 0)
  {
    animation->_currentTime = this->_restartAt;
    this->_occurences--;
  }
  else if (this->_occurences == -1)
  {
    animation->_currentTime = this->_restartAt;
  }
}

RLAWait::RLAWait(unsigned int start) : RgbLedAnimationStep(start)
{
  
}
void RLAWait::run(RgbLed* animation)
{
  //Do nothing
}

RLAStop::RLAStop(unsigned int start) : RgbLedAnimationStep(start)
{
  
}
boolean RLAStop::isActive(RgbLed* animation)
{
  return (animation->_currentTime >= this->_start);
}
void RLAStop::run(RgbLed* animation)
{
  animation->stopAnimation();
}
