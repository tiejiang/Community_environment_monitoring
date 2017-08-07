#include "stm32f10x.h"
#include "lcd.h"
#include "English.h"

void LCD_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5| GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void set_SCE(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
}

void clear_SCE(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);
}

void set_RES(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
}

void clear_RES(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
}

void set_DC(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
}

void clear_DC(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
}

void set_SDIN(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
}

void clear_SDIN(void)
{
	GPIO_WriteBit(GPIOA	, GPIO_Pin_5, Bit_RESET);
}

void set_SCLK(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
}

void clear_SCLK(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
}
/************************************************
*函数名: LCD_write_byte							*
*参  数：data：写入的数据 					    *
*        command: 1-数据/0-命令					*
*功  能: 向5110LCD写入数据						*
*返回值：无                                     *
*备  注：无 									*
*日  期：2009/12/3								*
*************************************************/ 
void LCD_write_byte(unsigned char data, unsigned char command)
{
	unsigned char i;  	
    clear_SCE(); 	
	if(command)
		set_DC();
	else
		clear_DC();	
		
    					 
	for(i=0;i<8;i++)
	{ 
		if(data&0x80)
			set_SDIN();
		else
			clear_SDIN();
		data = data<<1;	
		clear_SCLK(); 			
		set_SCLK(); 		  
		clear_SCLK();		
	}	  
}

/************************************************
*函数名: LCD_init							    *
*参  数：无                  					*
*功  能: 初始化LCD						        *
*返回值：无                                     *
*备  注：无 									*
*日  期：2009/12/3								*
*************************************************/
void LCD_init(void)
{
	
	set_SCE();
	set_RES();
	_delay_Nus(200);
	
	clear_RES();  	
  	_delay_Nms(20);
  	set_RES();  
	_delay_Nus(200);
	
	LCD_write_byte(0x21,0);//LCD功能设置：芯片活动，水平寻址，使用扩展指令
	LCD_write_byte(0xd0,0);//设置VOP值，室温下的编程范围为3.00-10.68
	//Vlcd=3.06+(VOP)*0.06,本例VOP为0B0101 0000为十进制的80，Vlcd=7.86V
	LCD_write_byte(0x20,0);//LCD功能设置：芯片活动，水平寻址，使用基本指令
	LCD_write_byte(0x0C,0);//设定显示配置:普通模式
	set_SCE();
}

/************************************************
*函数名: LCD_set_XY							    *
*参  数：X:列					                *
*        Y:行         			                *
*功  能: 选择写入液晶位置						*
*返回值：无                                     *
*备  注：无 									*
*日  期：2009/12/3								*
*************************************************/
void LCD_set_XY(unsigned char X, unsigned char Y)
{
	LCD_write_byte(0x40 | Y, 0);// 行
	LCD_write_byte(0x80 | X, 0);// 列
	set_SCE();
} 

/************************************************
*函数名: LCD_clear							    *
*参  数：无         			                *
*功  能: 清屏              						*
*返回值：无                                     *
*备  注：无 									*
*日  期：2009/12/3								*
*************************************************/
void LCD_clear(void)
{
	unsigned char t;
	unsigned char k;
	LCD_set_XY(0,0);
	for(t=0;t<6;t++)
	{ 
		for(k=0;k<84;k++)
		{ 
			LCD_write_byte(0x00,1);	 						
		} 
	}
	set_SCE();
}

/************************************************
*函数名: LCD_write_char							*
*参  数：c:需写入的字符         			    *
*功  能: 写入字符命令              				*
*返回值：无                                     *
*备  注：无 									*
*日  期：2009/12/3								*
*************************************************/
void LCD_write_char(unsigned char c)
{
	unsigned char c_line;
	c-=32;
	for (c_line=0; c_line<6; c_line++)
	LCD_write_byte(font6x8[c][c_line], 1);	
}

/************************************************
*函数名: LCD_write_String						*
*参  数：X:设置写入字符串列位置         		*
*		 Y:设置写入字符串行位置					*
*		 *s:写入字符串首地址                    *
*功  能: LCD显示字符串             				*
*返回值：无                                     *
*备  注：无 									*
*日  期：2009/12/3								*
*************************************************/
void LCD_write_String(unsigned char X,unsigned char Y,char *s)
{
	LCD_set_XY(X,Y);
	while (*s) 
	{
		LCD_write_char(*s);
		s++;
	}
} 
void LCD_write_Char(unsigned char X,unsigned char Y,char s)
{
	LCD_set_XY(X,Y);
	LCD_write_char(s);  		
}

/************************************************
*函数名: LCD_write_chinese_character			*
*参  数：X:设置写入汉字列位置         		    *
*		 Y:设置写入汉字行位置					*
*		 ch_with:写入汉字宽度                   *
*        num:写入汉字数量 						*
*        ch_line:汉字间行间距				    *
*		 ch_row:汉字间列间距					*
*功  能: LCD显示汉字             				*
*返回值：无                                     *
*备  注：无 									*
*日  期：2009/12/3								*
*************************************************/
/*
void LCD_write_chinese_character(unsigned char X, unsigned char Y,unsigned char ch_with,
                                 unsigned char num,unsigned char ch_line,unsigned char ch_row)
{
    unsigned char i,n;

    LCD_set_XY(X,Y);                             //设置初始位置

    for (i=0;i<num;)
      {
      	for (n=0; n<ch_with*2; n++)              //写一个汉字
      	  { 
      	    if (n==ch_with)                      //写汉字的下半部分
      	      {
      	        if (i==0) LCD_set_XY(X,Y+1);
      	        else
      	           LCD_set_XY((X+(ch_with+ch_row)*i),Y+1);
              }
      	    LCD_write_byte(chinese_character[ch_line+i][n],1);
      	  }
      	i++;
      	LCD_set_XY((X+(ch_with+ch_row)*i),Y);
      }
  }
 */



void LCD_write_chinese_character(unsigned char X, unsigned char Y,char n)
{
	int i;
	LCD_set_XY(X,Y);
	for(i = 0;i < 12; i++)
	{
		LCD_write_byte(chinese_character[n][i],1);
	}
	LCD_set_XY(X,Y+1);
	for(i = 12; i < 24; i++)
	{
		LCD_write_byte(chinese_character[n][i],1);
	}
}
