#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#define LED_PIN_DDR DDRD
#define LED_PIN_PORT PORTD
#define LED_PIN PD3

extern volatile uint16_t adcValue;

void ADC_Init();
void delay_ms(int ms);
uint16_t ADC_Read(uint8_t ch);
void USART_Init(unsigned int ubrr);
void USART_Transmit(unsigned char data);
void USART_SendString(const char* str);

#endif // TIMER_H