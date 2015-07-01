#ifndef POWER_UP_H_INCLUDED
#define POWER_UP_H_INCLUDED

#include <avr/io.h>

#define POWCONST 999

enum PowerUP{RED_TANK, MUSHROOM, BIG_DADY, NO_POWERUP};  
typedef enum PowerUP powerUp_type;

powerUp_type currentPowerUp;

/**
 * If 1 big roomba power up is active
 */
extern uint8_t bigRoombaActive;

/**
 * If 1 mushroom power up is active
 */
extern uint8_t mushroomActive;

/**
 * Used for counting which state we have when showing the randomize
 * symbol on the display
 */
extern int16_t powerUpDisplayCounter;

/**
 * \brief Get an random power up and show corresponding information 
 * 			on display
 * 
 * \param tickCountRand the tickCount of a wheel, used for initializing
 * 			the random generator
 */
void getPowerUp(uint16_t tickCountRand);

/**
 * \brief Perform actions corresponding to shoot a specific power up
 */
void shootPowerUp(void);

/**
 * \brief Called from the timer, when 
 * 		the time for a power up to be active is over
 */
void powerUpIsOver(void);

/**
 * \brief Used for setting the display,
 * 			when power up is generated and steering must not be blocked
 * 			and to finish the process of getting a power up
 */
void showRandomizeSign(void);

#endif
