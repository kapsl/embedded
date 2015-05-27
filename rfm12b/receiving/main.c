

#include "RFM12B.h"
#include "usart.h"

#include <stdio.h>

#define NODEID           1  //network ID used for this unit
#define NETWORKID       99  //the network ID we are on
#define SERIAL_BAUD 115200


void my_sleep(uint16_t ms) {
  uint16_t cycles;

  while (--ms) {
    cycles = 800;
    while (--cycles);
  }
}




int main(void) {

  Initialize(NODEID, NETWORKID);

  uart_init();
  send_string("Listening...");
  for (;;) {
    if (ReceiveComplete())
    {
      if (CRCPass())
      {
        send_string("[");
        send_value(GetSender());
        send_string("] ");
        for (uint8_t i = 0; i < *DataLen; i++)
          send_byte((char)Data[i]);

        if (ACKRequested())
        {
          SendACK();
          send_string(" - ACK sent");
        }
      }
      else
        send_string("BAD-CRC");

      send_string("\r\n");
    }
  }
  return 0;

}
