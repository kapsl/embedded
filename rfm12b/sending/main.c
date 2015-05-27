#include "RFM12B.h"
#include "usart.h"
#include <stdio.h>

#define NODEID        2  //network ID used for this unit
#define NETWORKID    99  //the network ID we are on
#define GATEWAYID     1  //the node ID we're sending to
#define ACK_TIME     50  // # of ms to wait for an ack
#define SERIAL_BAUD  115200


int interPacketDelay = 50; //wait this many ms between sending packets
char input = 0;

uint8_t sendSize = 0;
char payload[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890~!@#$%^&*(){}[]`|<>?+=:;,.";
char requestACK = 0;

void my_sleep(uint16_t ms) {
  uint16_t cycles;
  while (--ms) { 
    cycles = 800;
    while (--cycles);
  }
}

char waitForAck(void) {

  my_sleep(ACK_TIME);
  if (ACKReceived(GATEWAYID))
    return 1;
  return 0;
}



int main(void) {

  uart_init();
  Initialize(NODEID, NETWORKID);
  send_string("Transmitting...\n\n");
  for (;;) {
    send_string("Sending[");
    send_value(sendSize + 1);
    send_string("]:");
    for (uint8_t i = 0; i < sendSize + 1; i++)
      send_byte((char)payload[i]);


    requestACK = !(sendSize % 3); //request ACK every 3rd xmission



    Send(GATEWAYID, payload, sendSize + 1, requestACK);

    if (requestACK)
    {
      send_string(" - waiting for ACK...");
      if (waitForAck()) send_string("ok!");
      else send_string("nothing...");
    }


    sendSize = (sendSize + 1) % 88;
    send_string("\n\r");
    my_sleep(interPacketDelay);
  }
  return 0;

}
