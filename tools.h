#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED

#include <avr/io.h>

// Timer
#define CTC_1024 0x0D

#define TIMER_VAL_ADDR *(volatile uint16_t *) 0x88

void my_msleep(uint16_t ms);
uint16_t concat_bytes(uint8_t *data);

#endif
