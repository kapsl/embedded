#ifndef USART_H_INCLUDED
#define USART_H_INCLUDED

#include <avr/io.h>

/**
 * \brief Initialize usart communication with roomba
 */
void usart_init_roomba(void);

/**
 * \brief send a byte to the roomba
 */
void send_byte_roomba(uint8_t byte);

/**
 * \brief Receive a byte from roomba
 */
uint8_t receive_byte_roomba(void);

/**
 * \brief Initialize connection for screen command on pc (for debugging)
 */
void usart_init(void);

/**
 * \brief Send byte to screen
 */
void send_byte(uint8_t byte);

/**
 * \brief Receive byte from screen
 */
uint8_t receive_byte(void);

/**
 * \brief Sends a string to the screen terminal
 * 
 * \param string the string to write on the output
 */
void sendString(char * string);

/**
 * \brief Send a value to screen
 */
void send_value(uint8_t value);

#endif
