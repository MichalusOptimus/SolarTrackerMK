
#include "stm32l4xx.h"
#include "USART3.h"

unsigned char wp = 0, stateFrame = 1, i=0;

// Union -> represent float as 4 bytes
union
{
	unsigned char bytes[4];
	float a;
}floatToBytes;


void USART3_Init()
{

	// 1. Enable USART3 clock and GPIOB GPIOD (PB10 -> TX, PD9 - RX)
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIODEN;
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART3EN;

	// 2. Configure LPUART pins as alternate functions
	GPIOB->MODER &= ~GPIO_MODER_MODE10_0; 	// Set PB10 as Alternate function, 0 write in MODER7[1]
	GPIOD->MODER &= ~GPIO_MODER_MODE9_0; 	// Set PD9 as Alternate function, 0 write in MODER8[1]

	GPIOB->AFR[1] |= GPIO_AFRH_AFSEL10_0 | GPIO_AFRH_AFSEL10_1 | GPIO_AFRH_AFSEL10_2;	//PB10 -> Select alternate function in AFRH (USART3_TX), AF7 -> bits(11:10:9:8)=(0:1:1:1)
	GPIOD->AFR[1] |= GPIO_AFRH_AFSEL9_0 | GPIO_AFRH_AFSEL9_1 | GPIO_AFRH_AFSEL9_2;		//PD9 -> Select alternate function in AFRH (USART3_RX), Table 16 datasheet STM32L496

	// 3. Set word length, baud rate
	// USART3->CR1 |=				// Default word length -> 1 start bit, 8 data bits
	// USART3->CR2 |=				// Default 1 STOP bits
	USART3->BRR = F_CLK/BAUDRATE;	// 115200 baudrate

	// 4. Enable UART, transmitter and receiver
	USART3->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;

	// 5. Enable USART interrupt
	USART3->CR1 |= USART_CR1_RXNEIE; //| USART_CR1_IDLEIE;
	NVIC_EnableIRQ(USART3_IRQn);

}



// Write data to send buffer
void dataToBuffer(float *var, unsigned char *bufSend)
{
	unsigned char j,i;
	for(j=0;j<NUM_OF_FLOATS_SEND_USART;j++)
	{
	floatToBytes.a = var[j];
	for(i=0;i<sizeof(float);i++)
	{
		bufSend[i+sizeof(float)*j] = floatToBytes.bytes[3-i];
	}
	}
}

void USART3_IRQHandler()
{
	// Receive buffer not empty
	// Frame checking
	if(USART3->ISR & USART_ISR_RXNE)
	{
		unsigned char data = USART3->RDR;
		switch(stateFrame)
		{
		case 1:
			if(data == '#')
			{
				bufReceive[wp]=data;
				stateFrame = 2;
			}
		break;
		case 2:
			if(data == '#')
			{
				bufReceive[wp]=data;
				stateFrame = 3;
			}
			else stateFrame = 1;
		break;
		case 3:
			if(data == '#')
			{
				bufReceive[wp]=data;
				stateFrame = 4;
			}
			else stateFrame = 1;
		break;
		case 4:
				bufReceive[wp]=data;
				if(wp == 31) stateFrame = 5;
		break;
		case 5:
			if(data == '!')
			{
				bufReceive[wp]=data;
				stateFrame = 1;
				receivedMessageFromUSART = 1;
			}
			else stateFrame = 1;
		break;
		}
		if(wp++ >= sizeof(bufReceive)-1) wp = 0;
	}


	/*if(USART3->ISR & USART_ISR_IDLE)
	{
		USART3_ReadMessage(wp);
		wp=0;
		USART3->ICR |= USART_ICR_IDLECF;
	}*/
}


// Send a single char
void USART3_SendChar(unsigned char c)
{
	while(!(USART3->ISR & USART_ISR_TXE));	// wait for transmission complete
	USART3->TDR = c; 						//load char to register
}

// Send string from uint8_t array
void USART3_SendData(float *var, unsigned char *buf, char len)
{
	dataToBuffer(var,buf);
	for(i=0;i<len;i++)
	{
		USART3_SendChar(buf[i]);
	}
}

// Send string from char array
void USART3_SendStringChar(const char *str)
{
	while(*str) USART3_SendChar(*str++);
}

// Parsing data from PC
void parseDataFromUSART(char *cnt, float *kp, float *ki, float *kd, float *sat1, float *sat2, float *K, float *tempTarget)
{
	*cnt = bufReceive[3];	//counter message

	for(i=1;i<=NUM_OF_FLOATS_RECV_USART;i++)
	{

		floatToBytes.bytes[0] = bufReceive[4*i];
		floatToBytes.bytes[1] = bufReceive[4*i+1];
		floatToBytes.bytes[2] = bufReceive[4*i+2];
		floatToBytes.bytes[3] = bufReceive[4*i+3];

		if(i == 1) *kp = floatToBytes.a;
		else if(i == 2) *ki = floatToBytes.a;
		else if(i == 3) *kd = floatToBytes.a;
		else if(i == 4) *sat1 = floatToBytes.a;
		else if(i == 5) *sat2 = floatToBytes.a;
		else if(i == 6) *K = floatToBytes.a;
		else if(i == 7) *tempTarget = floatToBytes.a;
	}

}

