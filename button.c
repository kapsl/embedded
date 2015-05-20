#include "button.h"
#include "tools.h"

void initializeButton() {
	// Set pull up resistor
	PORTB = (1 << PORTB4);
	PORTH = (1 << PORTH6);
}

uint8_t button_pressed(uint8_t button) {
	// If bitmask says that both buttons should be pressed
	if ((button==SW12_PRESSED)&&((PINB&button)==0x00)&&((PINH & button) == 0x00)) {
		return 1;
	} else if (button == (1 << PORTB4) && (PINB & button) == 0x00) {
		return 1;
	} else if (button == (1 << PORTH6) && (PINH & button) == 0x00) {
		return 1;
	} 
	
	return 0;
}

void button_wait(uint8_t button) {
	uint8_t flag = 1;
	
	while (flag) {
		if (button_pressed(button)) {
			flag = 0;
		}
	}
}
