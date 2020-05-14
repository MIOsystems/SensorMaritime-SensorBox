/*
 * GPS.h
 *
 *  Created on: 13 May 2020
 *      Author: Beheerder
 */

#ifndef HEADER_GPS_H_
#define HEADER_GPS_H_

#include <DAVE.h>


bool outputBufferReady;

bool gpsOutputready;


bool msMinInitialized;

bool msMaxInitialized;


bool minInitialized;

bool maxInitialized;


int messageState;

char *headerStateMachineFilter; //= "$GNRMC\n";
char  receivedSentence[200];
char  receivedSentence2[200];

//uint8_t* currentSentenceBuffer;

uint8_t* firstSentenceBuffer;
uint8_t* secondSentenceBuffer;

//char  sprintString[200];



//uint8_t* outputBufferStart = receivedSentence;
//uint8_t* outputBufferCurrent = receivedSentence;
//uint8_t* outputBufferEnd = receivedSentence+199;




uint8_t* outputBufferStart; //= currentSentenceBuffer;
uint8_t* outputBufferCurrent; //= currentSentenceBuffer;
uint8_t* outputBufferEnd; //= currentSentenceBuffer+199;

uint8_t* secondOutputBufferStart; //= currentSentenceBuffer;
uint8_t* secondOutputBufferCurrent; //= currentSentenceBuffer;
uint8_t* secondOutputBufferEnd; //= currentSentenceBuffer+199;


void swapGPSBuffers();

void gpsInterrupt();

#endif /* HEADER_GPS_H_ */
