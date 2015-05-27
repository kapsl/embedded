#ifndef POWER_UP_H_INCLUDED
#define POWER_UP_H_INCLUDED

#include <avr/io.h>

enum PowerUP{RED_TANK, MUSHROOM, BIG_DADY, NO_POWERUP};  
typedef enum PowerUP powerUp_type;

powerUp_type currentPowerUp;
uint8_t bigRoombaActive;
uint8_t mushroomActive;

/**
 * \brief Called, when roomba drives over power up point
 */
powerUp_type getPowerUp(uint16_t tickCountRand);
void shootPowerUp(void);
void powerUpIsOver(void);

#endif
