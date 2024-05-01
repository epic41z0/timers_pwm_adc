#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

// Global variable to store received character
volatile char receivedChar;
volatile uint16_t adcValue = 0;

void USART_Init(unsigned int ubrr) {
    UBRR0H = (unsigned char)(ubrr >> 8); // Set baud rate
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enable receiver and transmitter
    UCSR0C = (3 << UCSZ01) | (3 << UCSZ00); // Set frame format: 8 data, 1 stop bit
}

void ADC_Init() {
    ADMUX = (1 << REFS0); // AVCC with external capacitor at AREF pin
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Enable ADC, prescaler 128
}

bool USART_Available() {
    return (UCSR0A & (1 << RXC0)); // Return 1 if unread data in buffer
}

char USART_Receive() {
    while (!(UCSR0A & (1 << RXC0))); // Wait for data to be received
    return UDR0; // Get and return received data from buffer
}

void USART_Transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait for empty transmit buffer
    UDR0 = data; // Put data into buffer, sends the data
}

void USART_SendString(const char* str) {
    while (*str) {
        USART_Transmit(*str++); // Send each character in the string
    }
}
