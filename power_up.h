#ifndef POWER_UP_H_INCLUDED
#define POWER_UP_H_INCLUDED

#include <avr/io.h>

enum PowerUP{RED_TANK, MUSHROOM, BIG_DADY, NO_POWERUP};  
typedef enum PowerUP powerUp_type;

powerUp_type currentPowerUp;

extern uint8_t bigRoombaActive;
extern uint8_t mushroomActive;
extern int16_t powerUpDisplayCounter;

void getPowerUp(uint16_t tickCountRand);
void shootPowerUp(void);
void powerUpIsOver(void);
void showRandomizeSign(void);

#endif
