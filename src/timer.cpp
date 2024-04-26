#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_PIN 3 // Använd pinne 3 för LED
#define LED_DDR DDRD // Använd port D för LED
#define LED_PORT PORTD // Använd port D för LED

volatile bool ledState = false; // Variabel för att hålla reda på LED-status

void setupTimer() {
  // Konfigurera timer1
  TCCR1B |= (1 << WGM12); // CTC mode
  TIMSK1 |= (1 << OCIE1A); // Tillåt compare A-interrupt
  
  // Räkna ut värdet för att uppnå en periodtid på 200 ms
  // F_CPU = 16 MHz
  // Prescaler = 1024
  // Önskad periodtid = 200 ms = 0.2 s
  // Timer räknar upp med (F_CPU / Prescaler)
  // Antal ticks för att uppnå 200 ms = (0.2 s * (F_CPU / Prescaler)) - 1
  // Antal ticks = (0.2 * (16000000 / 1024)) - 1 = 312.5
  // Avrunda till närmaste heltal, får 312
  OCR1A = 312; // Sätt compare-värdet för att generera compare A-interrupt var 200 ms
  
  // Aktivera timer med prescaler 1024
  TCCR1B |= (1 << CS12) | (1 << CS10);
}

// ISR för compare A-interrupt
ISR(TIMER1_COMPA_vect) {
  // Toggla LED-status
  ledState = !ledState;
  
  // Uppdatera LED baserat på nya statusen
  if (ledState) {
    LED_PORT |= (1 << LED_PIN); // Tänd LED
  } else {
    LED_PORT &= ~(1 << LED_PIN); // Släck LED
  }
}

int main() {
  // Sätt LED-pinne som output
  LED_DDR |= (1 << LED_PIN);
  
  // Aktivera globala interrupts
  sei();
  
  // Konfigurera timer
  setupTimer();
  
  while (1) {
    // Huvudprogrammet körs automatiskt av ISR (interrupt service routine)
  }
  
  return 0;
}
