#include "timer.h"
#include "led.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define VREF 5.0        // Referensspänning för ADC
#define ADC_MAX 1023.0  // Maximalt värde för ADC med 10 bits upplösning
#define THRESHOLD 0.04  // Spänningsgräns för LED-effekt
 #define BUFFER_SIZE 50 // Storlek på mottagningsbuffert för USART

void parseCommand(char* command);

int main() {    

    
    USART_Init(MYUBRR);//startar USART
    LEDTimer_Init();
    sei();// Enable global interrupts

    char buffer[BUFFER_SIZE] = {0};
    int bufIndex = 0;

    while (1) {
        if (USART_Available()) {
            char receivedChar = USART_Receive();
            if (receivedChar == '\r' || receivedChar == '\n') {
                USART_SendString("Kommando mottaget!\n");  // Feedback till användaren
                buffer[bufIndex] = '\0';
                parseCommand(buffer);        
                memset(buffer, 0, BUFFER_SIZE);  // Rensa bufferten helt
                bufIndex = 0;
            } else if (bufIndex < BUFFER_SIZE - 1) {
                buffer[bufIndex++] = receivedChar;
            } else {
                USART_SendString("Error: Buffer overflow\n");
                memset(buffer, 0, BUFFER_SIZE);
                bufIndex = 0;
            }
        }
    }

    return 0;
}
