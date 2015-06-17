#include "tools.h"
#include "timer.h"
#include "avr/interrupt.h"
#include "power_up.h"
#include "usart.h"

uint8_t timer_counter_1 = -1;
uint8_t max_seconds_1 = 0;

// TODO make one function out of the different timers

/**
 * \brief Initialize the first timer, used for longer times
 * 
 * \param seconds how many seconds till the timer is triggered
 * 
 * TODO ask if cli / sei like this is ok
 */
void initializeTimer1(uint16_t seconds) {
	cli();
	//sendString("Timer initialized...");
	
	max_seconds_1 = (int8_t) seconds;
	timer_counter_1 = 0;
	
	// Make 1 second
	uint16_t timerval = 15625;
	
	// Initalize timer with CTC and 1024 as divider
	TCCR1B = CTC_1024;
	
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
	
	//sendString("Interrupt 1 ...");
	
	// DO stuff when timer is on
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

/**
 * \brief Initialize timer 2, used for short times.
 * 
 * \param ms How many milliseconds till timer is triggered
 */
void initializeTimer2(uint16_t ms) {
	cli();
	//sendString("Timer 2 initialized...");
	
	uint16_t timerval = ms / 1000 * 15625;
	
	// Initalize timer with CTC and 1024 as divider
	TCCR3B = CTC_1024;
	
	OCR3AH = (timerval & 0xFF00) >> 8;
	OCR3AL = (timerval & 0x00FF);
		
	// Activate interrupts
	TIMSK3 = 0x03;
	
	sei();
}

/**
 * \brief Stop counting for timer 2
 */
void stopTimer2() {
	TCCR3B = 0x0;
}

/**
 * \brief Timer 2 is triggered
 */
ISR(TIMER3_COMPA_vect) {
	cli();
	
	sendString("Interrupt Timer 2 ...");
	
	timer2Triggered();
	
	sei();
}
