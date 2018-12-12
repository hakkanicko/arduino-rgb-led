# arduino-rgb-led
A tiny lib to animate a RGB led in arduino programs


## Example

```cpp
#include "RgbLed.h"

const byte PIN_LED_R  = PD6;
const byte PIN_LED_G  = PD5;
const byte PIN_LED_B  = PD3;

void setup() {
  led.init(PIN_LED_R,PIN_LED_G,PIN_LED_B);
  led.setColor(Color(0,0,0));

  RgbLedAnimationStep* steps[]={
    new RLASetTo(0, Color(0,0,0)),
    new RLAFadeTo(100, Color(0,0,255)),
    new RLAFadeTo(1100, Color(255,0,0)),
    new RLAFadeTo(2100, Color(0,255,0)),
    new RLAFadeTo(3100, Color(0,0,255)),
    new RLALoopTo(4100, 1100, -1)
  };
  led.setAnimation(sizeof(steps)/sizeof(RgbLedAnimationStep*), steps);
  led.startAnimation();
}

void loop() {
  led.runAnimation();
}

```