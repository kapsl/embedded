#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED

#include <avr/io.h>

/**
 * \brief Wait for
 * 
 * \param ms milliseconds
 */
void my_msleep(uint16_t ms);

/**
 * \brief Concats two bytes from the array data [0] and [1] to one 
 * 		  binary value
 * 
 * \param data high and low byte on [0] and [1] of the array
 * 
 * \return the concatenated high and low byte as value
 */
uint16_t concat_bytes(uint8_t *data);

#endif
