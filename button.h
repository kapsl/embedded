#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <avr/io.h>

// The buttons
#define SW1_PRESSED (1 << PINB4)
#define SW2_PRESSED (1 << PINH6)
#define SW12_PRESSED (1 << PINB4) | (1 << PINH6)

void initializeButton(void);
uint8_t button_pressed(uint8_t button);
void button_wait(uint8_t button);

#endif
