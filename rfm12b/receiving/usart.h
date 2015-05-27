#ifndef USART_H
#define	USART_H
#define UARTSPEED 16
#include <avr/io.h>

void uart_init(void);
void send_byte(uint8_t byte);
void send_string(char *byte);
void send_value(uint8_t value);
uint8_t receive_byte(void);



#endif