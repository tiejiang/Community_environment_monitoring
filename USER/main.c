#include "stm32f10x.h" 
#include "LCD.h"  
#include "SysTick.h"
#include "DHT11.h"
#include "key.h"
#include "sim900.h"
#include "mq_2.h"

#define GPIO_LED GPIOA
#define PIN_LED GPIO_Pin_8
/*GPIO端口配置结构体变量定义*/
GPIO_InitTypeDef GPIO_InitStructure;

char content[16]="W:     T:     K ";
char NUM1[14]={0x22,0x31,0x35,0x30,0x37,0x35,0x35,0x33,0x34,0x38,0x36,0x31,0x22};
char NUM2[14]={0x22,0x31,0x35,0x31,0x38,0x34,0x30,0x33,0x32,0x34,0x36,0x32,0x22};

u16 count = 0;
u8 MessageFlag = 1;
u8 key = 0;  //键盘扫描值
char  SetTemp = 35;   //设定温度
char  SetHuti = 60;   //设定湿度
u8 Key_Flag = 0;  //当Key_Flag = 1选定温度并上调，2选定温度下调，3选定湿度上调，4选定温度下调 5退出
u8 Stop_Flag = 0;
u8 Action_Flag = 0;
DHT11_Data_TypeDef DHT11_Data;
void Show_Dth11(void);	 //显示DTH11的数据
void Show_SetTemp_Huti(void);//显示设定的温湿度
void Key1_show(void);   //显示按键的状态
void Show_Mq(void);
void Show_Mq1(void);
void System_Init(void);
void Show_Init(void);
void Tem_Action(void);
void Hui_Action(void);
void Key_Action(void);
int main(void)
{
    
	System_Init();
	Show_Init();
	while(1)
	{
		if( Read_DHT11(&DHT11_Data)==SUCCESS)										 //“\”表示转向一下行
			{
				Show_Dth11(); 
				if(DHT11_Data.temp_int > SetTemp )	
					{
					   Tem_Action();
					   content[6] = '+';
					   Action_Flag = 1;
					}			
				else
					{
					   content[6] = '-';	
					}
				if(DHT11_Data.humi_int > SetHuti)
					{
					   content[13] = '+';
					   Action_Flag = 1;
					}
				else
					{
					   content[13] = '-';
					}
			}
		if(Action_Flag == 1 && MessageFlag == 1 )
		{
			Tem_Action();
			Action_Flag = 0;
			Show_Init();
		    MessageFlag = 0;
			count = 0;
			Action_Flag = 0;
		}
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) == 0 && MessageFlag == 1)  //监测MQ-2所检测参数是否正常
	  		{
	   		    Hui_Action();
//				Show_Init();
				Show_Mq();
				MessageFlag = 0;
				count = 0;
	   		}
		else
			{
				Show_Mq1();
			}
		key = KEY_Scan();
		Key_Action();		  
		Key1_show();
		_delay_Nms(10); 
		if(MessageFlag == 0)		   //防止重复报警
		{
		  if(count++ >= 500)
		  {
		   MessageFlag = 1;
		   Action_Flag = 0;
		  }
		}
	}
	  
}
void Key1_show(void)
{
    if(Key_Flag == 5)
		{
		  Stop_Flag = 0;
		  Key_Flag = 0;  //清除标志位
		  LCD_write_chinese_character(24,0,5);//(
	      LCD_write_chinese_character(48,0,6);//)
		  LCD_write_chinese_character(24,2,5);//(
	      LCD_write_chinese_character(48,2,6);//)
		  Show_SetTemp_Huti();
		}
	switch(Key_Flag)
	  {
	  	 case 1:
		 		{
				//	LCD_write_String(24,0,"<<");	有黑影
				    LCD_write_Char(24,0,'<');
					LCD_write_Char(30,0,'<');
					LCD_write_chinese_character(48,0,6);//)
		  			LCD_write_chinese_character(24,2,5);//(
	     			LCD_write_chinese_character(48,2,6);//)
		 		}break;
		case 2:
				{
				//	LCD_write_String(48,0,">>");
				    LCD_write_Char(48,0,'>');
					LCD_write_Char(54,0,'>');
					LCD_write_chinese_character(24,0,5);//(
					LCD_write_chinese_character(24,2,5);//(
	                LCD_write_chinese_character(48,2,6);//)
				}	break;
		case 3:
				{
					//LCD_write_String(24,2,"<<")	;
					LCD_write_Char(24,2,'<');
					LCD_write_Char(30,2,'<');
					LCD_write_chinese_character(24,0,5);//(
	                LCD_write_chinese_character(48,0,6);//)
					LCD_write_chinese_character(48,2,6);//)
				}   break;
		case 4:
				{
				//	LCD_write_String(48,2,">>");
				    LCD_write_Char(48,2,'>');
					LCD_write_Char(54,2,'>');
					LCD_write_chinese_character(24,0,5);//(
	      			LCD_write_chinese_character(48,0,6);//)
		  			LCD_write_chinese_character(24,2,5);//(
				}	break;
		default:
				break;
	  }
}
void Show_Dth11(void)
{
	char GetTemp[4];
	char Gethumi[4];
	u8 i = 0;
	GetTemp[0] = DHT11_Data.temp_int /10 + 0x30;
	GetTemp[1] = DHT11_Data.temp_int %10+ 0x30;
  	GetTemp[2] = '.';
	GetTemp[3] = DHT11_Data.temp_deci /10+ 0x30;
	//GetTemp[4] = DHT11_Data.temp_deci %10+ 0x30;

	Gethumi[0] = DHT11_Data.humi_int /10+ 0x30;
	Gethumi[1] = DHT11_Data.humi_int %10+ 0x30;
  	Gethumi[2] = '.';
	Gethumi[3] = DHT11_Data.humi_deci /10+ 0x30;
	//Gethumi[4] = DHT11_Data.humi_deci %10+ 0x30;
  	LCD_write_String(60,0,GetTemp);
	for(i = 0;i<4;i++)
	  {
	  	GetTemp[i] = 0;
	  }
	LCD_write_String(60,2,Gethumi);
	for(i = 0;i<4;i++)
	  {
	  	Gethumi[i] = 0;
	  }
}

void Show_SetTemp_Huti()
{
	char temp[2];
	char humi[2];
	temp[0] = SetTemp /10 + 0x30;
	temp[1] = SetTemp %10 + 0x30;

	humi[0] = SetHuti /10 + 0x30;
	humi[1] = SetHuti %10 + 0x30;

	LCD_write_Char(33,0,temp[0]);
    LCD_write_Char(39,0,temp[1]);

	LCD_write_Char(33,2,humi[0]);
    LCD_write_Char(39,2,humi[1]);	


}
void Show_Mq(void)
{
	LCD_write_chinese_character(0,4,7);//空
	LCD_write_chinese_character(12,4,8);//气
	LCD_write_chinese_character(24,4,2);//：
	LCD_write_chinese_character(36,4,11);//不
	LCD_write_chinese_character(48,4,9);//正	
	LCD_write_chinese_character(60,4,10);//常	
}
void Show_Mq1(void)
{
	LCD_write_chinese_character(0,4,7);//空
	LCD_write_chinese_character(12,4,8);//气
	LCD_write_chinese_character(24,4,2);//：  	
	LCD_write_chinese_character(36,4,9);//正	
	LCD_write_chinese_character(48,4,10);//常	
	LCD_write_chinese_character(60,4,12);
}

void System_Init(void)
{
	SysTick_Init();    //滴答定时器初始化
	LCD_GPIO_Init();   //5110管脚初始化
	DHT11_GPIO_Config(); 
	KEY_Init();       //键盘引脚初始化 
	LCD_init();        //5110初始化
	MQ_GPIO_Config();
}
void Show_Init(void)
{
  	LCD_clear();	
  	LCD_write_chinese_character(0,0,0);//温
	LCD_write_chinese_character(12,0,1);//度
	LCD_write_chinese_character(24,0,5);//(
	LCD_write_chinese_character(48,0,6);//)
  	LCD_write_chinese_character(0,2,4);//湿
	LCD_write_chinese_character(12,2,1);//度
	LCD_write_chinese_character(24,2,5);//(
	LCD_write_chinese_character(48,2,6);//)	 
	LCD_write_chinese_character(0,4,7);//空
	LCD_write_chinese_character(12,4,8);//气
	LCD_write_chinese_character(24,4,2);//：
	LCD_write_chinese_character(36,4,9);//正
	LCD_write_chinese_character(48,4,10);//常
	//LCD_write_String(25,0,SetTemperature);
    Show_SetTemp_Huti();
}
void Alarm(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//打开GPIO时钟
	//将LED的4个引脚分别设置为推挽输出
	GPIO_InitStructure.GPIO_Pin = PIN_LED;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_LED, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, PIN_LED); 
}
void Tem_Action(void)
{
	content[2] = DHT11_Data.temp_int /10 + 0x30;
	content[3] = DHT11_Data.temp_int %10+ 0x30;	//温度
	content[4] = '.';
	content[5] = 0x30;	 
	content[9] = DHT11_Data.humi_int /10+ 0x30;
	content[10] = DHT11_Data.humi_int %10+ 0x30;   //湿度
	content[11] = '.';
	content[12] = 0x30;
	content[15] = 'T';   //湿度
//	Inid_Sim900();
//	MESSAGE(NUM2,content);	
/*设定温度小于实际温度则PA_8引脚做出反应*/
    Alarm();

}
void Hui_Action(void)
{
	Show_Mq();				
	content[2] = DHT11_Data.temp_int /10 + 0x30;
	content[3] = DHT11_Data.temp_int %10+ 0x30;	//温度
	content[4] = '.';
	content[5] = 0x30;	 
	content[9] = DHT11_Data.humi_int /10+ 0x30;
	content[10] = DHT11_Data.humi_int %10+ 0x30;   //湿度
	content[11] = '.';
	content[12] = 0x30;
	content[15] = 'F';   //湿度
   /*
	Inid_Sim900();
	MESSAGE(NUM2,content);
	*/
/*MQ-2检测到的参数异常则PA_8引脚做出反应*/
   Alarm();
	
}
void Key_Action(void)
{
	switch(key)
  	{
	   case 1:
	   		{
			   Stop_Flag = 1;
			   if(Key_Flag == 2)
			   	  SetTemp--;
			   else if(Key_Flag == 4)
			   	  SetHuti--; 
				  						
			   Show_SetTemp_Huti();	 
			  
			} break;
	   case 2:
	   		{
			   Stop_Flag = 1;
			   if(Key_Flag == 1)
			      SetTemp++;
			   else if(Key_Flag == 3)
			      SetHuti++;

			   Show_SetTemp_Huti();						  
			} break;
	   case 3:
	   		{
			 Key_Flag++;
			 if(Stop_Flag == 1)
			    Key_Flag = 5; 	
			 
			} break;
	   default:
	          break;
	}
}

