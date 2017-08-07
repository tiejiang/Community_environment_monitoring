#ifndef __LCD_H
#define __LCD_H
 
void LCD_GPIO_Init(void);
void _delay_Nus(u32 nTime);
void _delay_Nms(u32 nTime);
void LCD_write_byte(unsigned char data, unsigned char command);
void LCD_write_byte(unsigned char data, unsigned char command);

void LCD_clear(void);
void LCD_init(void);
void LCD_write_String(unsigned char X,unsigned char Y,char *s);
void LCD_write_chinese_character(unsigned char X, unsigned char Y,char n);
void LCD_write_Char(unsigned char X,unsigned char Y,char s);

#endif

