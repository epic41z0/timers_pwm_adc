#ifndef LED_H
#define LED_H

#include <avr/io.h>

// Declare global variables
extern volatile uint8_t ledPower;
extern volatile uint16_t ledPeriod;

// Function prototypes
void LEDTimer_Init();
void LEDTimer_SetPower(uint8_t power);
void LEDTimer_SetPeriod(uint16_t period);
void LEDTimer_EnablePWM();
void parseCommand(char* command);

#endif // LEDTIMER_H