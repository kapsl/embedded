#include "tools.h"
#include "timer.h"
#include "avr/interrupt.h"
#include "power_up.h"
#include "usart.h"

/**
 * How often do we have to let the timer run, till we reached
 * the number of seconds we want
 */
uint8_t timer_counter_1 = TIMECONST;

/**
 * After how many seconds should the timer call the callback function
 */
uint8_t max_seconds_1 = 0;

void initializeTimers() {
	// Set first LED to on
	PORTF = PORTF | PORTF1;
	
	// Initalize timer 1 with CTC and 1024 as divider
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1  = 0;

	// Make 1 second
	uint16_t timerval = 15625;

	OCR1AH = (timerval & 0xFF00) >> 8;
	OCR1AL = (timerval & 0x00FF);
  
	TCCR1B = CTC_1024;
  
	// Sets the timer overflow interrupt enable bit 	
	TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
	
	//TCCR1B |= _BV(CS12) | _BV(CS10);
	
	// Set first LED to off
	PORTF = 0;
}

void startTimer1(uint16_t seconds) {
	max_seconds_1 = (int8_t) seconds;
	timer_counter_1 = 0;
	
	// Sets the timer overflow interrupt enable bit 
	//TIMSK3 = _BV(TOIE1);
	//sei();
	
	// Start timer
	//TCCR1B |= _BV(CS12) | _BV(CS10);
}

/**
 * \brief Timer1 is triggered, but we need additional logic to 
 * 			support longer times
 */
ISR(TIMER1_COMPA_vect) {
	cli();
	PORTF = PORTF | PORTF2;
	
	//sendString("Interrupt 1 ...");
	
	// Do stuff when timer is triggered
	if (timer_counter_1 < max_seconds_1 - 1  && timer_counter_1 != TIMECONST) {
		timer_counter_1++;		
	} else if (timer_counter_1 == max_seconds_1 - 1) {
		powerUpIsOver();
		
		// Stop timer
		//TCCR3B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12)); // Clears all clock selects bits
		
		timer_counter_1 = TIMECONST;
	}	
	PORTF = 0;
	
	sei();
}
