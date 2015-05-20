#include <avr/io.h>
#include "tools.h"
#include "button.h"
#include "usart.h"
#include <ctype.h>
#include "roomba.h"

int main(int argc, const char* argv[]) {
	initializeButton();
	usart_init();
	usart_init_roomba();
	initializeRoomba();
	
	while (1) {	
		if (read_button(HOUR)) {
				drive_with_p_regler();
		}
	} 
    
    return 0;
}
