#ifndef REMOTECONTROL_H_INCLUDED
#define REMOTECONTROL_H_INCLUDED

#include <avr/io.h>

#define LEFT ((uint8_t) (141))
#define RIGHT ((uint8_t) (143))
#define FORWARD ((uint8_t) (139))
#define BACKWARDS ((uint8_t) (140))
#define POWER ((uint8_t) (0x8A))
#define NR_2 ((uint8_t) (0x83))
#define NR_4 ((uint8_t) (0x85))
#define NR_5 ((uint8_t) (0x86))
#define NR_6 ((uint8_t) (0x87))
#define NR_8 ((uint8_t) (0x89))

enum RemoteSignal{RACCELERATE, RBRAKE, RLEFT, RRIGHT, RSHOOT, RNOTHINGPRESSED};  
typedef enum RemoteSignal remoteSignal;

remoteSignal getRemoteSignal(uint16_t remoteSignalData);

#endif
