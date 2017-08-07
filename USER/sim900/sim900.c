#include "sim900.h"
#include "lcd.h"
#include <string.h>
void Inid_Sim900(void)
{
   Sim900_RCC();
   Sim900_Usart2_NVIC();
   Sim900_GPIO_Init();
   Sim900_Usart_Con();
//   GPIO_ResetBits(GPIOA,GPIO_Pin_8);
//   _delay_Nms(1000);
//   GPIO_SetBits(GPIOA,GPIO_Pin_8);
    LCD_clear();
    USART2_Puts("AT\r\n");  //握手
	LCD_write_String(0,0,"AT");
    _delay_Nms(100);
    while(!Hand("OK"))
	      {
		  CLR_RX();
		  USART2_Puts("AT\r\n");  //握手
		  _delay_Nms(100);	       
	      }
	 CLR_RX();
	 _delay_Nms(100); 
	  while(!Hand("OK"))       //等待设置成功
		  {
		  CLR_RX(); 		           
          USART2_Puts("AT+CMGR=1\r\n");//设置当有新短信到来时提示 
		  LCD_write_String(0,1,"AT+CMGR");
		  _delay_Nms(100);  			      
		  }
	 CLR_RX();
	 _delay_Nms(100); 
	 while(!Hand("OK"))       //等待设置成功
		  { 	
		  CLR_RX();	           
          USART2_Puts("AT+CNMI=2,1\r\n");//设置当有新短信到来时提示 
		  LCD_write_String(0,2,"AT+CNMI");
		  _delay_Nms(100);  			      
		  }
	 CLR_RX();
	 _delay_Nms(100); 
	 while(!Hand("OK"))       //等待设置成功
		  { 	
		  CLR_RX();	           
          USART2_Puts("AT+CMGF=1\r\n");//设置短信格式 
		  LCD_write_String(0,3,"AT+CMGF");
		  _delay_Nms(100);  			      
		  }
	 CLR_RX();
	// _delay_Nms(100); 

}
void Sim900_Usart2_NVIC(void)
{
 	NVIC_InitTypeDef NVIC_InitStructure; 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void Sim900_GPIO_Init(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;	  

  /* Configure USART2 Tx (PA.02) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    
  /* Configure USART2 Rx (PA.03) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}
void Sim900_RCC(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); 
}
void Sim900_Usart_Con(void)
{
	USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART2, ENABLE);
}
void CLR_RX(void)
{
	char k;
    for(k=0;k<200;k++)    //将缓存内容清零
	    {
			RX[k] = 0;
		}
    seat = 0;                    //接收字符串的起始存储位置
	
}

u8 Hand(u8 *a)
{ 
    if(strstr((char *)RX,(char *)a)!=NULL)
	    return 1;
	else
		return 0;
}	
void MESSAGE(char *NUM,char *CH)
{	 
     CLR_RX();
	 _delay_Nms(100);
     USART2_Puts("AT+CMGS=");
     USART2_Puts(NUM);
	 USART2_Puts("\r\n"); 
     _delay_Nms(100);
	 
     while(!Hand(">"))

	 CLR_RX();
	 _delay_Nms(100);
     USART2_Puts(CH);
	 USART_SendData(USART2, 0x1a);  		          
	 while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 
     _delay_Nms(100);
	 while(!Hand("OK"));
	 CLR_RX();
	 LCD_write_String(0,4,"OK");
	 _delay_Nms(1000);
	 LCD_clear();
	 
//	 GPIO_ResetBits(GPIOA,GPIO_Pin_8);
//     _delay_Nms(1000);
//     GPIO_SetBits(GPIOA,GPIO_Pin_8);   //关机


}

void USART2_Puts(char * str)
{    
     while(*str)  
          {        
		  USART_SendData(USART2, *str++);  		          
		  while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);    
		  }
}
void USART2_PutHex(char Hex)
{    
        
		  USART_SendData(USART2, Hex);  		          
		  while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);    

}
