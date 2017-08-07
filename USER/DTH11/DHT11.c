
#include "DHT11.h"

/*
 * ��������DHT11_GPIO_Config
 * ����  ������DHT11�õ���I/O��
 * ����  ����
 * ���  ����
 */
void DHT11_GPIO_Config(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����GPIOD������ʱ��*/
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);

	/*ѡ��Ҫ���Ƶ�GPIOD����*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��GPIOD*/
  	GPIO_Init(GPIOB, &GPIO_InitStructure);		  

	/* ����GPIOD12	*/
	GPIO_SetBits(GPIOB, GPIO_Pin_0);	 
}

/*
 * ��������DHT11_Mode_IPU
 * ����  ��ʹDHT11-DATA���ű�Ϊ����ģʽ
 * ����  ����
 * ���  ����
 */
static void DHT11_Mode_IPU(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;

	  	/*ѡ��Ҫ���Ƶ�GPIOD����*/	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;

	   /*��������ģʽΪ��������ģʽ*/ 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 

	  /*���ÿ⺯������ʼ��GPIOD*/
	  GPIO_Init(GPIOB, &GPIO_InitStructure);	 
}

/*
 * ��������DHT11_Mode_Out_PP
 * ����  ��ʹDHT11-DATA���ű�Ϊ���ģʽ
 * ����  ����
 * ���  ����
 */
static void DHT11_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	 	/*ѡ��Ҫ���Ƶ�GPIOD����*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/*���ÿ⺯������ʼ��GPIOD*/
  	GPIO_Init(GPIOB, &GPIO_InitStructure);	 	 
}


static uint8_t Read_Byte(void)
{	  

   	 uint8_t i, temp=0;

	 for(i=0;i<8;i++)    
	 {
	 
	   /*ÿbit��50us�͵�ƽ���ÿ�ʼ����ѯֱ���ӻ����� ��50us �͵�ƽ ����*/  
    	while(DHT11_DATA_IN()==Bit_RESET);
		 
		 /*DHT11 ��27~28us�ĸߵ�ƽ��ʾ��0������70us�ߵ�ƽ��ʾ��1����
	 		 ͨ�����60us��ĵ�ƽ��������������״̬*/

		_delay_Nus(6); //��ʱ60us		   	  
	
		  if(DHT11_DATA_IN()==Bit_SET)//60us����Ϊ�ߵ�ƽ��ʾ���ݡ�1��
		   {
		   	/*��ѯֱ���ӻ�������ʣ��� 30us �ߵ�ƽ����*/
   				 while(DHT11_DATA_IN()==Bit_SET);

				 temp|=(uint8_t)(0x01<<(7-i));  //�ѵ�7-iλ��1 
			
		   }
		   else	 //60us��Ϊ�͵�ƽ��ʾ���ݡ�0��
		   {			   
		   	  temp&=(uint8_t)~(0x01<<(7-i)); //�ѵ�7-iλ��0
		   }
	 }
	  return temp;
}

uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data)
{  
	/*���ģʽ*/
   DHT11_Mode_Out_PP();
   /*��������*/
   DHT11_DATA_OUT(LOW);
   /*��ʱ18ms*/
   _delay_Nms(18);

   /*�������� ������ʱ30us*/
   DHT11_DATA_OUT(HIGH); 

   _delay_Nus(3);   //��ʱ30us
   
 	/*������Ϊ���� �жϴӻ���Ӧ�ź�*/ 
   DHT11_Mode_IPU();

 /*�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������*/   
   if(DHT11_DATA_IN()==Bit_RESET)   //T !   
    {
  
	
	  /*��ѯֱ���ӻ����� ��80us �͵�ƽ ��Ӧ�źŽ���*/  
	    while(DHT11_DATA_IN()==Bit_RESET);
	  	 
	  /*��ѯֱ���ӻ������� 80us �ߵ�ƽ �����źŽ���*/
	    while(DHT11_DATA_IN()==Bit_SET);

	  /*��ʼ��������*/   
	 	 DHT11_Data->humi_int= Read_Byte();
								
		 DHT11_Data->humi_deci= Read_Byte();
				
		 DHT11_Data->temp_int= Read_Byte();

		 DHT11_Data->temp_deci= Read_Byte();
				 		
		 DHT11_Data->check_sum= Read_Byte();
		 								  

		 	/*��ȡ���������Ÿ�Ϊ���ģʽ*/
  		 DHT11_Mode_Out_PP();
		   /*��������*/
		 DHT11_DATA_OUT(HIGH);

		   /*����ȡ�������Ƿ���ȷ*/
		 if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)
		  	return SUCCESS;
		  else 
		  	return ERROR;
   }
   else
   	{		
			return ERROR;
		}
   
}

	  


/*************************************END OF FILE******************************/
