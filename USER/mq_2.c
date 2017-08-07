# include "mq_2.h"

/*
 * MQ-2 特点：
1、具有信号输出指示。
2、双路信号输出（模拟量输出及TTL电平输出）
3、TTL输出有效信号为低电平。（当输出低电平时信号灯亮，可直接接单片机）                 
4、模拟量输出0~5V电压，浓度越高电压越高。
5、对液化气，天然气，城市煤气有较好的灵敏度。
6、具有长期的使用寿命和可靠的稳定性
7、快速的响应恢复特性

 */

void MQ_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  	/*开启按键端口（PB）的时钟*/
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_10);
}
