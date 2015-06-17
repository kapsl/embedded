#include "tools.h"
#include "timer.h"
#include "avr/interrupt.h"
#include "power_up.h"
#include "usart.h"

/**
 * How often do we have to let the timer run, till we reached
 * the number of seconds we want
 */
uint8_t timer_counter_1 = -1;

/**
 * After how many seconds should the timer call the callback function
 */
uint8_t max_seconds_1 = 0;

/**
 * \brief Initialize the two timers
 */
void initializeTimers() {
	// Initalize timer 1 with CTC and 1024 as divider
	//TCCR1B = CTC_1024;
	//sei();
}

/**
 * \brief Initialize the first timer, used for longer times
 * 
 * \param seconds how many seconds till the timer is triggered
 */
void startTimer1(uint16_t seconds) {
	cli();
	max_seconds_1 = (int8_t) seconds;
	timer_counter_1 = 0;
	
	TCCR1B = CTC_1024;
	
	// Make 1 second
	uint16_t timerval = 15625;

	OCR1AH = (timerval & 0xFF00) >> 8;
	OCR1AL = (timerval & 0x00FF);
		
	// Activate interrupts
	TIMSK1 = 0x03;
	sei();
}

/**
 * \brief Timer1 is triggered, but we need additional logic to 
 * 			support longer times
 */
ISR(TIMER1_COMPA_vect) {
	cli();
	
	sendString("Interrupt 1 ...");
	
	// Do stuff when timer is triggered
	if (timer_counter_1 < max_seconds_1 - 1  && timer_counter_1 != -1) {
		timer_counter_1++;		
	} else if (timer_counter_1 == max_seconds_1 - 1) {
		powerUpIsOver();
		
		// Stop timer
		TCCR1B = 0x0;
		
		timer_counter_1 = -1;
	}	
	
	sei();
}
