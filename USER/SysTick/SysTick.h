#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void SysTick_Init(void);
void _delay_Nus(__IO u32 nTime);
void _delay_Nms(u32 nTime);

#endif /* __SYSTICK_H */
