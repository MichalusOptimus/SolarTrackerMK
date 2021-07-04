/*
 * USART3.h
 *
 *  Created on: 10.02.2021
 *      Author: Michal
 *
 */

#include "stm32l4xx.h"

#define F_CLK 80000000					// System clock frequency
#define BAUDRATE 115200					// USART3 baudrate
#define NUM_OF_FLOATS_RECV_USART 7 		// Number of floats from PC
#define NUM_OF_FLOATS_SEND_USART 14 	// Number of floats to send

unsigned char bufSend[4+NUM_OF_FLOATS_SEND_USART*4];		// 4 additional signs -> start frame ##, end frame !!
unsigned char bufReceive[20];
char receivedMessageFromUSART;	// flag


// Configuration USART3 registers
void USART3_Init();

// Write data to send buffer
void dataToBuffer(float *var, unsigned char *bufSend);

// Send a single char
void USART3_SendChar(unsigned char c);

// Send data written in bytes (char)
//void USART3_SendData(unsigned char *str, char len);
void USART3_SendData(float *var, unsigned char *buf, char len);

// Send string from char array
void USART3_SendStringChar(const char *str);

// Parsing data from PC
void parseDataFromUSART(char *cnt, float *kp, float *ki, float *kd, float *sat1, float *sat2, float *K, float *tempTarget);





