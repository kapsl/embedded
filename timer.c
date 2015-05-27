#include "tools.h"
#include "timer.h"
#include "avr/interrupt.h"
#include "power_up.h"

uint8_t timer_counter = 0;
uint8_t max_seconds = 0;

/**
 * TODO
 */
void initializeTimer(uint16_t ms) {
	uint16_t timerval = ms / 1000 * 15625;
	
	// Initalize timer with CTC and 1024 as divider
	TCCR1B = CTC_1024;
	
	OCR1AH = (timerval & 0xFF00) >> 8;
	OCR1AL = (timerval & 0x00FF);
		
	// Activate interrupts
	TIMSK1 = 0x03;
}

ISR(TIMER1_COMPA_vect) {
	cli();
	
	// DO stuff when timer is on
	if (timer_counter < max_seconds) {
		timer_counter++;
	} else {
		powerUpIsOver();
	}	
	
	sei();
}
