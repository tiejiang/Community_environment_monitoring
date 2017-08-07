#ifndef __SIM900_H
#define __SIM900_H
#include "stm32f10x.h"
void Sim900_Usart2_NVIC(void);
void Sim900_GPIO_Init(void);
void Sim900_RCC(void);
void Sim900_Usart_Con(void);
void USART2_PutHex(char Hex);
void USART2_Puts(char * str);
void MESSAGE(char *NUM,char *CH);
void CLR_RX(void);
void Inid_Sim900(void);
u8 Hand(u8 *a);
void MESSAGE(char *NUM,char *CH);
extern void _delay_Nms(u32 nTime);
extern char RX[200];
extern int seat;
#endif
