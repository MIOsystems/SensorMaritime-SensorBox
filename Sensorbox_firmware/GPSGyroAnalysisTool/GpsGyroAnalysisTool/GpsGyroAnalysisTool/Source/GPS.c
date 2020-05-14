/*
 * GPS.c
 *
 *  Created on: 13 May 2020
 *      Author: Beheerder
 */

#include "Header/GPS.h"


void initGPS(){

	headerStateMachineFilter = "$GNRMC\n";

}


void swapGPSBuffers(){

	uint8_t * tempSentence = firstSentenceBuffer;
	firstSentenceBuffer = secondSentenceBuffer;
	secondSentenceBuffer = tempSentence;

	//currentSentenceBuffer = firstSentenceBuffer;

	outputBufferStart = firstSentenceBuffer;
	outputBufferCurrent = firstSentenceBuffer;
	outputBufferEnd = firstSentenceBuffer+199;

	secondOutputBufferStart = secondSentenceBuffer;
	secondOutputBufferCurrent = secondSentenceBuffer;
	secondOutputBufferEnd = secondSentenceBuffer+199;
}

void gpsInterrupt(){


	UART_Receive(&UART_0, outputBufferCurrent, 1);


	if(headerStateMachineFilter[messageState] == *outputBufferCurrent){ //sequence correct go to next position;
		messageState++;
		outputBufferCurrent++;
		if(messageState == 7){
			swapGPSBuffers();
			outputBufferCurrent = outputBufferStart;
			messageState = 0;
			outputBufferReady = true;

		}

	}
	else if (messageState  < 6){ // sequence incorrect oh mi god
		outputBufferCurrent = outputBufferStart;
		messageState = 0;

	}
	else{ //data part of sequence just increment here;
		outputBufferCurrent++;
	}



//	if(messageState > 0){

//	}

}
