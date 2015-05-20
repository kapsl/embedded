#include "usart.h"
#include "tools.h"

void usart_init_roomba() {
	// Baudrate
	UBRR3L = 0x10;
	UBRR3H = 0x00;
	
	// Double speed
	UCSR3A |= (1 << U2X3);
	
	// Enable receiver and transmitter
	UCSR3B |= (1 << RXEN3) | (1 << TXEN3);
		
	// Set character size
	UCSR3B &= ~(0 << UCSZ32);
	UCSR3C |= (1 << UCSZ31) | (1 << UCSZ30);
	
	// Set ansychronous
	UCSR3C &= ~(0 << UMSEL31) & ~(0 << UMSEL30);
	
	// No parity
	UCSR3C &= ~(0 << UPM31) & ~(0 << UPM30);
	
	// 1 Stop bit
	UCSR3C &= ~(0 << USBS3);	
}

void send_byte_roomba(uint8_t byte) {	
	while (!(UCSR3A & (1 << UDRE3)));
	
	UDR3 = byte;
}

uint8_t receive_byte_roomba() {
	while(!(UCSR3A & (1 << RXC3)));
	
	return UDR3;
}

void usart_init() {
	// Baudrate
	UBRR0L = 0x10;
	UBRR0H = 0x00;
	
	// Double speed
	UCSR0A |= (1 << U2X0);
	
	// Enable receiver and transmitter
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
		
	// Set character size
	UCSR0B |= (0 << UCSZ02);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
	
	// Set ansychronous
	UCSR0C |= (0 << UMSEL01) | (0 << UMSEL00);
	
	// No parity
	UCSR0C |= (0 << UPM01) | (0 << UPM00);
	
	// 1 Stop bit
	UCSR0C |= (0 << USBS0);	
}

void send_byte(uint8_t byte) {	
	while (!(UCSR0A & (1 << UDRE0)));
	
	UDR0 = byte;
}

uint8_t receive_byte() {
	while(!(UCSR0A & (1 << RXC0)));
	
	return UDR0;
}

/**
 * \brief Sends a string to the screen terminal
 * 
 * \param string the string to write on the output
 */
void sendString(char * string) {
	uint8_t a = 0;
	
	while(string[a] != '\0') {
		send_byte(string[a]);
		
		a++;
	}
}
