#include "timer.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define THRESHOLD 0.04  // Tröskelvärde för att betrakta spänningen som 0V

// Interrupt Service Routine för ADC
ISR(ADC_vect) {
    adcValue = ADC;  // Läs ADC-värdet från A0
}

int main() {
    char buffer[32];
    double voltage;

    ADC_Init();  // Initialisera ADC
    USART_Init(MYUBRR);  // Initialisera USART
    LED_PIN_DDR |= (1<<LED_PIN);  // Sätt LED-pinnen som utgång

// Aktivera ADC-interrupt
ADCSRA |= (1 << ADIE);
// Aktivera globala interrupt
sei();

    while (1) {
        // Starta ADC-konvertering
        ADCSRA |= (1 << ADSC);

        voltage = adcValue * (5.0 / 1023.0);  // Beräkna spänningen

        int voltage_int = (int)voltage;
        int voltage_frac = (int)(voltage * 100) % 100;  // Hämta de två första decimalplatserna
        sprintf(buffer, "ADC: %d, Spänning: %d.%02dV\n", adcValue, voltage_int, voltage_frac);  // Förbered strängen

        USART_SendString(buffer);  // Skicka strängen via USART

        if (voltage <= THRESHOLD) {
            LED_PIN_PORT |= (1<<LED_PIN);  // LED på
        } else {
            // Blinka LED med en frekvens som är omvänt proportionell mot spänningen
            LED_PIN_PORT |= (1<<LED_PIN);  // LED på
            delay_ms(100 * voltage / 5);  // Fördröjning proportionell mot spänningen
            LED_PIN_PORT &= ~(1<<LED_PIN);  // LED av
            delay_ms(100 * voltage / 5);  // Fördröjning proportionell mot spänningen
        }
    }


    return 0;

}