#ifndef __KEY_H
#define __KEY_H
#include "sys.h"

#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//读取按键1
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//读取按键2 
#define KEY3  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)//读取按键3



#define KEY_S4		4
#define KEY_S3  	3
#define KEY_S2  	2
#define KEY_S1   	1


void KEY_Init(void);
u8 KEY_Scan(u8);  	//按键扫描函数

#endif
