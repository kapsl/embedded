#include <stdio.h>
#include <stdlib.h>
#include "radio.h"
#include "RFM12B.h"
#include "usart.h"
#include "tools.h"

void initializeRadio(uint8_t nr) {	
	node_id = nr;
	//Initialize(nr, NETWORKID);
}

void sendRadio(char payload, uint8_t timesToResend) {
	// We want an ack
	uint8_t requestACK = 1;
	//uint8_t interPacketDelay = 50; //wait this many ms between sending packets
	
	sendString("Transmitting...\r\n");
	
	char payloadArray[1];
	payloadArray[0] = payload;
    
    // Send to node_id 1 or 2 (the opposite)
	Send(((node_id % 2) + 1), payloadArray, 1, requestACK);
	
	sendString("Transmitted...\r\n");
	
	// TODO test with ack etc. bad checksum ...
	if (requestACK) {
		sendString(" Waiting for ACK...");
      
		if (waitForAck()) {
			sendString("Ack OK!");  
		} else {
			if (timesToResend > 0) {
				sendString("No ack - resending...");
				sendRadio(payload, timesToResend - 1);
			} else {
				sendString("No ack - resending cancelled...");
			}
		}
    }
	
	//my_msleep(interPacketDelay);
}

char waitForAck() {
	uint64_t waitCounter = 4000;
	
	while(waitCounter > 0) {
		if (ACKReceived((node_id % 2) + 1)) {
			return 1;
		}
	
		waitCounter--;
	}
    
	return 0;
}

char receiveRadio() {
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
