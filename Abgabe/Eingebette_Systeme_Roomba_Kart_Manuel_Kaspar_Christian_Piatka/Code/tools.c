#include <tools.h>
#include <util/delay_basic.h>
#include "avr/interrupt.h"

void my_msleep(uint16_t ms) {
	for (uint16_t a = 0; a < ms; a++) {
		_delay_loop_2(4000);
	}
}

uint16_t concat_bytes(uint8_t *data) {
	uint16_t value = data[0] << 8;
	value |= data[1];
	
	return value;
}


