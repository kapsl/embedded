#include "tools.h"
#include "timer.h"
#include "avr/interrupt.h"
#include "power_up.h"
#include "usart.h"

uint8_t timer_counter_1 = -1;
uint8_t max_seconds_1 = 0;

uint8_t timer_counter_2 = -1;
uint8_t max_seconds_2 = 0;

// TODO make one function out of the different timers

/**
 * TODO
 */
void initializeTimer1(uint16_t seconds) {
	cli();
	//sendString("Timer initialized...");
	
	max_seconds_1 = (int8_t) seconds;
	timer_counter_1 = 0;
	
	//uint16_t timerval = ms / 1000 * 15625;
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

ISR(TIMER1_COMPA_vect) {
	cli();
	
	sendString("Interrupt 1 ...");
	
	// DO stuff when timer is on
	if (timer_counter_1 < max_seconds_1 - 1  && timer_counter_1 != -1) {
		timer_counter_1++;		
	} else if (timer_counter_1 == max_seconds_1 - 1) {
		powerUpIsOver();
		
		TCCR1B = 0x0;
		
		timer_counter_1 = -1;
	}	
	
	sei();
}

/**
 * TODO make possibility to use ms
 */
void initializeTimer2(uint16_t seconds) {
	cli();
	//sendString("Timer initialized...");
	
	max_seconds_2 = seconds;
	timer_counter_2 = 0;
	
	//uint16_t timerval = ms / 1000 * 15625;
	// Make 1 second
	uint16_t timerval = 15625;
	
	// Initalize timer with CTC and 1024 as divider
	TCCR3B = CTC_1024;
	
	OCR3AH = (timerval & 0xFF00) >> 8;
	OCR3AL = (timerval & 0x00FF);
		
	// Activate interrupts
	TIMSK3 = 0x03;
	
	sei();
}

ISR(TIMER3_COMPA_vect) {
	cli();
	
	sendString("Interrupt 3 ...");
	
	// DO stuff when timer is on
	if (timer_counter_2 < max_seconds_2 - 1) {
		timer_counter_2++;		
	} else if (timer_counter_2 == max_seconds_2 - 1 && timer_counter_2 != -1) {
		sendString("Timer 3 reached ...");
		
		TCCR3B = 0x0;
		
		timer_counter_2 = -1;
	}	
	
	sei();
}
