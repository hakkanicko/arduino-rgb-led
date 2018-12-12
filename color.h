#include <stdint.h>
#include "Arduino.h"

#ifndef __COLOR_H
#define __COLOR_H


class Color{

private:
  byte r;
  byte g;
  byte b;
  boolean gamma;
public:
  Color();
  Color(byte r, byte g, byte b);
  Color(String hex);
  void setRGB(byte r, byte g, byte b);
  void setHSV(float h, float s, float v);
  void setHex(String hex);
  byte getR();
  byte getG();
  byte getB();
  void setGamma(boolean a);
  boolean getGamma();
  String toString();
};

#endif
