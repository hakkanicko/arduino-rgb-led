#include "color.h"
#include "Logger.h"

int StrToHex(String str)
{

}

Color::Color()
{
}


Color::Color(String hex)
{
  this->setHex(hex);
}


Color::Color(byte r, byte g, byte b)
{
  this->setRGB(r,g,b);
}

void Color::setRGB(byte r, byte g, byte b)
{
  this->r = r;
  this->g = g;
  this->b = b;
}

void Color::setHex(String hex)
{
  char a[3];
  hex.toCharArray(a, 3);
  this->r = (byte) strtol(a, 0, 16);

  hex.substring(2).toCharArray(a, 3);
  this->g = (byte) strtol(a, 0, 16);

  hex.substring(4).toCharArray(a, 3);
  this->b = (byte) strtol(a, 0, 16);
}

void Color::setHSV(float h, float s, float v)
{
  float f,p,q,t;
  int i;
  h = max(0.0, min(360.0, h));
  s = max(0.0, min(100.0, s));
  v = max(0.0, min(100.0, v));
  
  s /= 100.0;
  v /= 100.0;
  
  if(s == 0) {
    // Achromatic (grey)
    this->r = this->g = this->b = round(v*255);
    return;
  }

  h /= 60; // sector 0 to 5
  i = floor(h);
  f = h - i; // factorial part of h
  p = v * (1.0 - s);
  q = v * (1.0 - s * f);
  t = v * (1.0 - s * (1.0 - f));
  switch(i) {
    case 0:
      this->r = round(255*v);
      this->g = round(255*t);
      this->b = round(255*p);
      break;
    case 1:
      this->r = round(255*q);
      this->g = round(255*v);
      this->b = round(255*p);
      break;
    case 2:
      this->r = round(255*p);
      this->g = round(255*v);
      this->b = round(255*t);
      break;
    case 3:
      this->r = round(255*p);
      this->g = round(255*q);
      this->b = round(255*v);
      break;
    case 4:
      this->r = round(255*t);
      this->g = round(255*p);
      this->b = round(255*v);
      break;
    default: // case 5:
      this->r = round(255*v);
      this->g = round(255*p);
      this->b = round(255*q);
    }
}

byte Color::getR()
{
    return this->r;
}

byte Color::getG()
{
    return this->g;  
}

byte Color::getB()
{
   return this->b;  
}

void Color::setGamma(boolean a){
  this->gamma = a;
}

boolean Color::getGamma(){
  return this->gamma;
}

String Color::toString()
{
  return "R : " + String(this->getR()) + " - " + "G : " + String(this->getG()) + " - "+ "R : " + String(this->getB()) + " ";
}
