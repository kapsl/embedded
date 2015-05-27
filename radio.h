#ifndef RADIO_H_INCLUDED
#define RADIO_H_INCLUDED

#include <avr/io.h>

//#define NODEID        1  //network ID used for this unit
//#define NODEID        2  //network ID used for this unit
#define NETWORKID    99  //the network ID we are on
//#define GATEWAYID     1  //the node ID we're sending to
#define ACK_TIME     50  // # of ms to wait for an ack
#define SERIAL_BAUD  115200

#define RED_TANK_SHOT 'r'

uint8_t node_id;

void initializeRadio(uint8_t nr);
void sendRadio(char payload);
char receiveRadio(void);
uint8_t waitForAck(void);

#endif
