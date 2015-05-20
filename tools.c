#include <tools.h>
#include <util/delay_basic.h>
#include "avr/interrupt.h"

void my_msleep(uint16_t ms) {
		for (uint16_t a = 0; a < ms; a++) {
			_delay_loop_2(4000);
		}
}

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
	// Do stuff when timer caused interrupt
	sei();
}

/**
 * \brief Concats two bytes from the array data [0] and [1] to one 
 * 		  binary value
 * 
 * \param data high and low byte on [0] and [1] of the array
 * \return the concatenated high and low byte as value
 */
uint16_t concat_bytes(uint8_t *data) {
	uint16_t value = data[0] << 8;
	value |= data[1];
	
	return value;
}


