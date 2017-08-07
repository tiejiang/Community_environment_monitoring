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
*������: LCD_write_byte							*
*��  ����data��д������� 					    *
*        command: 1-����/0-����					*
*��  ��: ��5110LCDд������						*
*����ֵ����                                     *
*��  ע���� 									*
*��  �ڣ�2009/12/3								*
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
*������: LCD_init							    *
*��  ������                  					*
*��  ��: ��ʼ��LCD						        *
*����ֵ����                                     *
*��  ע���� 									*
*��  �ڣ�2009/12/3								*
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
	
	LCD_write_byte(0x21,0);//LCD�������ã�оƬ���ˮƽѰַ��ʹ����չָ��
	LCD_write_byte(0xd0,0);//����VOPֵ�������µı�̷�ΧΪ3.00-10.68
	//Vlcd=3.06+(VOP)*0.06,����VOPΪ0B0101 0000Ϊʮ���Ƶ�80��Vlcd=7.86V
	LCD_write_byte(0x20,0);//LCD�������ã�оƬ���ˮƽѰַ��ʹ�û���ָ��
	LCD_write_byte(0x0C,0);//�趨��ʾ����:��ͨģʽ
	set_SCE();
}

/************************************************
*������: LCD_set_XY							    *
*��  ����X:��					                *
*        Y:��         			                *
*��  ��: ѡ��д��Һ��λ��						*
*����ֵ����                                     *
*��  ע���� 									*
*��  �ڣ�2009/12/3								*
*************************************************/
void LCD_set_XY(unsigned char X, unsigned char Y)
{
	LCD_write_byte(0x40 | Y, 0);// ��
	LCD_write_byte(0x80 | X, 0);// ��
	set_SCE();
} 

/************************************************
*������: LCD_clear							    *
*��  ������         			                *
*��  ��: ����              						*
*����ֵ����                                     *
*��  ע���� 									*
*��  �ڣ�2009/12/3								*
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
*������: LCD_write_char							*
*��  ����c:��д����ַ�         			    *
*��  ��: д���ַ�����              				*
*����ֵ����                                     *
*��  ע���� 									*
*��  �ڣ�2009/12/3								*
*************************************************/
void LCD_write_char(unsigned char c)
{
	unsigned char c_line;
	c-=32;
	for (c_line=0; c_line<6; c_line++)
	LCD_write_byte(font6x8[c][c_line], 1);	
}

/************************************************
*������: LCD_write_String						*
*��  ����X:����д���ַ�����λ��         		*
*		 Y:����д���ַ�����λ��					*
*		 *s:д���ַ����׵�ַ                    *
*��  ��: LCD��ʾ�ַ���             				*
*����ֵ����                                     *
*��  ע���� 									*
*��  �ڣ�2009/12/3								*
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
*������: LCD_write_chinese_character			*
*��  ����X:����д�뺺����λ��         		    *
*		 Y:����д�뺺����λ��					*
*		 ch_with:д�뺺�ֿ��                   *
*        num:д�뺺������ 						*
*        ch_line:���ּ��м��				    *
*		 ch_row:���ּ��м��					*
*��  ��: LCD��ʾ����             				*
*����ֵ����                                     *
*��  ע���� 									*
*��  �ڣ�2009/12/3								*
*************************************************/
/*
void LCD_write_chinese_character(unsigned char X, unsigned char Y,unsigned char ch_with,
                                 unsigned char num,unsigned char ch_line,unsigned char ch_row)
{
    unsigned char i,n;

    LCD_set_XY(X,Y);                             //���ó�ʼλ��

    for (i=0;i<num;)
      {
      	for (n=0; n<ch_with*2; n++)              //дһ������
      	  { 
      	    if (n==ch_with)                      //д���ֵ��°벿��
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
