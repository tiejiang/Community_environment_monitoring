#include "stm32f10x.h"
#include "key.h"
#include "SysTick.h"
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);		 

}
u8 KEY_Scan(void)
{	 
	static u8 key_up=1;//按键按松开标志	
 
 
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0))
	{
		_delay_Nms(5);//去抖动 
		key_up=0;
		if(KEY0==0)
		{
			 
			return 1;
		}
		else if(KEY1==0)
		{ 
	     	
			return 2;
		}
		else if(KEY2==0)
		{
	    	
			return 3;
		}
	}else if(KEY0==1&&KEY1==1&&KEY2==1)key_up=1; 	    
 
 
	return 0;// 无按键按下
}
