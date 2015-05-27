#include "usart.h"



void uart_init(void) {
	//Double Speed
	UCSR0A = 0x00;
	UCSR0A |= (1 << U2X0);
	//Enable Receiver
	UCSR0B = 0x00;
	UCSR0B |= (1 << RXEN0);
	//Enable Transmitter
	UCSR0B |= (1 << TXEN0);

	UCSR0C = 0x00;
	UCSR0C |= (1 << UCSZ01);
	UCSR0C |= (1 << UCSZ00);

	UBRR0L |= UARTSPEED;
	UBRR0H |= (UARTSPEED >> 8) & 0x07;

}

void send_byte(uint8_t byte) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = byte;
}

uint8_t receive_byte(void) {
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

void send_string(char *byte){
	while(*byte)
		send_byte(*byte++);
}



void send_value(uint8_t value){
	
	char buffer[10];
	sprintf(buffer,"%i",value);
	send_string(buffer);
}