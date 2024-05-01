
#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>
#include <avr/interrupt.h>

extern volatile bool ledState;

void initTimer1();
void initLED();
void setup();
void startUpLed();

#endif /* TIMER_H_ */
