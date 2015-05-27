#ifndef POWER_UP_H_INCLUDED
#define POWER_UP_H_INCLUDED

#include <avr/io.h>

enum PowerUP{RED_TANK, MUSHROOM, BIG_DADY};  
typedef enum PowerUP powerUp_type;

/**
 * \brief Called, when roomba drives over power up point
 */
powerUp_type getPowerUp(uint16_t tickCountRand);
void shootPowerUp(void);

#endif
