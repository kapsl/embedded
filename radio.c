#include <stdio.h>
#include <stdlib.h>
#include "radio.h"
#include "RFM12B.h"
#include "usart.h"
#include "tools.h"

void initializeRadio(uint8_t nr) {	
	node_id = nr;
	Initialize(nr, NETWORKID);
}

/**
 * TODO
 * 
 * \param payload - array with only one char!
 */
void sendRadio(char payload) {
	/**
	 * TODO ack doesn't work at the moment it seems that it is sending, but never received
	 */
	uint8_t requestACK = 0;
	//uint8_t interPacketDelay = 50; //wait this many ms between sending packets
	
	sendString("Transmitting...\r\n");
	
	char payloadArray[1];
	payloadArray[0] = payload;
    
    // Send to node_id 1 or 2 (the opposite)
	Send(((node_id % 2) + 1), payloadArray, 1, requestACK);
	
	sendString("Transmitted...\r\n");
	
	// TODO test with ack etc. bad checksum ...
	if (requestACK) {
		sendString(" - waiting for ACK...");
      
		if (waitForAck()) {
			sendString("ok!");  
		} else {
			sendString("No ack - resending...");
			sendRadio(payload);
		}
    }
	
	//my_msleep(interPacketDelay);
}

/**
 * TODO
 */
uint8_t waitForAck() {
	my_msleep(ACK_TIME);
  
	if (ACKReceived((node_id % 2) + 1)) {
		return 1;
	}
    
	return 0;
}

char receiveRadio() {
	//sendString("Receiving\r\n");
	if (ReceiveComplete()) {
		if (CRCPass()) {
			//uint8_t sender_node_id = GetSender();

			if (ACKRequested()) {
				SendACK();
				sendString(" - ACK sent");
			}
			
			return (char) Data[0];
		} else {
			sendString("BAD-CRC");
			
			return '0';
		}
    }
    
    return '0';
}
