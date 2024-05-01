#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED_PIN_DDR DDRD  // Data Direction Register för LED-pinnen
#define LED_PIN_PORT PORTD  // Portregister för LED-pinnen
#define LED_PIN PD3  // LED-pinnen

volatile uint16_t adcValue = 0;  // Global variabel för att lagra ADC-värdet

void ADC_Init() {
    ADMUX = (1<<REFS0);  // AVCC med extern kondensator vid AREF-pinnen
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);  // Aktivera ADC och sätt prescaler till 128
}

void delay_ms(int ms) {
    for (int i = 0; i < ms; i++) {
        _delay_ms(1);  // Fördröjning på 1 ms
    }
}

uint16_t ADC_Read(uint8_t ch) {
    ch &= 0b00000111;  // Behåll endast de tre sista bitarna
    ADMUX = (ADMUX & 0xF8)|ch;  // Rensa de tre sista bitarna innan ORing
    ADCSRA |= (1<<ADSC);  // Starta enkel konvertering
    while(!(ADCSRA & (1<<ADIF)));  // Vänta tills konverteringen är klar
    ADCSRA|=(1<<ADIF);  // Rensa ADIF genom att skriva ett till det
    return(ADC);  // Returnera ADC-värdet
}

void USART_Init(unsigned int ubrr) {
    UBRR0H = (unsigned char)(ubrr>>8);  // Sätt de högre 8 bitarna av baud rate-registret
    UBRR0L = (unsigned char)ubrr;  // Sätt de lägre 8 bitarna av baud rate-registret
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);  // Aktivera mottagare och sändare
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);  // Sätt stoppbit och karaktärsstorlek
}

void USART_Transmit(unsigned char data) {
    while (!(UCSR0A & (1<<UDRE0)));  // Vänta tills bufferten är tom
    UDR0 = data;  // Skicka data
}

void USART_SendString(const char* str) {
    while (*str) {
        USART_Transmit(*str++);  // Skicka varje tecken i strängen
    }
}