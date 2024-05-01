#include "led.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include <stdio.h>

volatile uint8_t ledPower = 128;  // Variable for LED power (0-255), initialized to a mid-range value for visibility
volatile uint16_t ledPeriod = 1000;  // Default period for LED blinking (1 second in ms)

void LEDTimer_Init() {
    // Timer1 CTC mode
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B = (1 << WGM12) | (1 << CS11);  // CTC mode, prescaler 8
    OCR1A = (F_CPU / 8000) * ledPeriod - 1;// Set compare value for period, rearranged to reduce error
    TIMSK1 = (1 << OCIE1A);  // Enable compare match interrupt

    // Timer2 Fast PWM mode
    DDRD |= (1 << PD3); // Set PD3 as output (OC2B)
    TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);  // Non-inverting mode, Fast PWM
    TCCR2B = (1 << CS21); // Prescaler 8
    OCR2B = ledPower;  // Set compare value for PWM

    sei();  // Enable interrupts globally
}

ISR(TIMER1_COMPA_vect) {
    static uint8_t toggle = 0;
    toggle = !toggle;
    if (toggle) {
        OCR2B = ledPower;  // Set to specified power level
    } else {
        OCR2B = 0;  // Turn off the LED
    }
}

void LEDTimer_SetPower(uint8_t power) {
    ledPower = power;
    OCR2B = power;  // Update PWM compare value immediately
}

void LEDTimer_SetPeriod(uint16_t period) {
    cli();  // Disable interrupts
    ledPeriod = period;

    uint32_t ticks = F_CPU / 1000;
    ticks *= period; // Calculate ticks without prescaler

    if (ticks < 65536) { // Can handle with prescaler 8
        TCCR1B = (1 << WGM12) | (1 << CS11); // CTC mode, prescaler 8
        OCR1A = ticks / 8 - 1;
    } else if (ticks < 262144) { // Use prescaler 64
        TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); // CTC mode, prescaler 64
        OCR1A = ticks / 64 - 1;
    } else if (ticks < 1048576) { // Use prescaler 256
        TCCR1B = (1 << WGM12) | (1 << CS12); // CTC mode, prescaler 256
        OCR1A = ticks / 256 - 1;
    } else { // Use prescaler 1024
        TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // CTC mode, prescaler 1024
        OCR1A = ticks / 1024 - 1;
    }

    TCNT1 = 0;  // Reset timer counter to ensure immediate effect
    sei();  // Enable interrupts
}


void parseCommand(char* command) {
    uint8_t power;
    uint16_t period;
    if (sscanf(command, "ledpowerfreq %hhu %u", &power, &period) == 2) {
        if (power > 255 || period < 200 || period > 5000) {
            USART_SendString("Error: Power must be 0-255 and period 200-5000ms.\n");
            return;
        }
        LEDTimer_SetPower(power);
        LEDTimer_SetPeriod(period);
        char response[100];
        snprintf(response, sizeof(response), "LED power set to %hhu and period set to %ums\n", power, period);
        USART_SendString(response);
    } else {
        USART_SendString("Invalid command format. Use: 'ledpowerfreq <power> <period>'.\n");
    }
}