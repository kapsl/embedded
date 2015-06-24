#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <avr/io.h>

// Timer
#define TIMER_CTC_ADDR *(volatile uint8_t *) 0x81
#define CTC_1024 0x0D
#define TIMER_VAL_ADDR *(volatile uint16_t *) 0x88

/**
 * \brief Initialize the timer with ctc 1024 and 1 second
 */
void initializeTimers(void);

/**
 * \brief Initialize the first timer, used for longer times
 * 
 * \param seconds how many seconds till the timer is triggered
 */
void startTimer1(uint16_t seconds);

#endif
