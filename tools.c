#include <tools.h>
#include <util/delay_basic.h>
#include "avr/interrupt.h"

void my_msleep(uint16_t ms) {
		for (uint16_t a = 0; a < ms; a++) {
			_delay_loop_2(4000);
		}
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


