#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <avr/io.h>

// Timer
#define TIMER_CTC_ADDR *(volatile uint8_t *) 0x81
#define CTC_1024 0x0D

//#define TIMER_INT_ADDR *(volatile uint8_t *) 0x6F
#define TIMER_VAL_ADDR *(volatile uint16_t *) 0x88

//uint8_t timer_counter;

void initializeTimer(uint16_t ms);

#endif
