# include "mq_2.h"

/*
 * MQ-2 �ص㣺
1�������ź����ָʾ��
2��˫·�ź������ģ���������TTL��ƽ�����
3��TTL�����Ч�ź�Ϊ�͵�ƽ����������͵�ƽʱ�źŵ�������ֱ�ӽӵ�Ƭ����                 
4��ģ�������0~5V��ѹ��Ũ��Խ�ߵ�ѹԽ�ߡ�
5����Һ��������Ȼ��������ú���нϺõ������ȡ�
6�����г��ڵ�ʹ�������Ϳɿ����ȶ���
7�����ٵ���Ӧ�ָ�����

 */

void MQ_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  	/*���������˿ڣ�PB����ʱ��*/
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_10);
}
