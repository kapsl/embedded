#ifndef USART_H_INCLUDED
#define USART_H_INCLUDED

#include <avr/io.h>

void usart_init(void);
void send_byte(uint8_t byte);
uint8_t receive_byte(void);

void usart_init_roomba(void);
void send_byte_roomba(uint8_t byte);
uint8_t receive_byte_roomba(void);

void sendString(char * string);
void send_value(uint8_t value);

#endif
