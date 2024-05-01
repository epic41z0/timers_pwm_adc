#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_PIN_PORT PORTD  // PORTD definieras för att styra utgången av digitala stiften
#define LED_PIN_DDR  DDRD   // Data direction register används för att sätta stiftet som utgång
#define LED_PIN_NUMBER PIND3  // PIND3 motsvarar digitalt pin 3

volatile bool ledState = false;

// Timer1 overflow interrupt service routine (ISR)
ISR(TIMER1_OVF_vect) {
    TCNT1 = 312; // Återställ timerpreload
    // Växla LED-läge
    ledState = !ledState;
}

void initTimer1() {
    TCCR1A = 0; // Initiera kontrollregistret A för Timer1
    TCCR1B = 0; // Initiera kontrollregistret B för Timer1
    TCCR1B |= (1 << CS11) | (1 << CS10); // Ange prescaler till 64
    TCNT1 = 31249; // Ladda förval Timer1 med värdet för en 200 ms period
    TIMSK1 |= (1 << TOIE1); // Aktivera Timer1 overflow interrupt
}

void initLED() {
    LED_PIN_DDR |= (1 << LED_PIN_NUMBER); // Sätt LED pin som utgång
}

void setup() {
    initTimer1(); // Initiera Timer1
    initLED(); // Initiera LED
    sei(); // Aktivera globala interrupts
}

// Funktion för att uppdatera LED-läge i main loopen
void startUpLed() {
    if (ledState) {
        LED_PIN_PORT |= (1 << LED_PIN_NUMBER); // Slå på LED
    } else {
        LED_PIN_PORT &= ~(1 << LED_PIN_NUMBER); // Slå av LED
    }
}
