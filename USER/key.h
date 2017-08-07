#ifndef __KEY_H
#define __KEY_H
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40011008
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x4001100C 

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define KEY0 PBin(6)   //PB6
#define KEY1 PBin(8)	//PB8 
#define KEY2 PBin(9)	//PB9
	 
void KEY_Init(void);//IO初始化
u8   KEY_Scan(void);  //按键扫描函数
#endif
