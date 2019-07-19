//#ifndef __MOTOR_H
//#define __MOTOR_H

//#define AIN2 PBout(15)
//#define AIN1 PBout(14)

//void MiniBalance_Motor_Init(void);


//#endif

#ifndef __MOTOR_H
#define __MOTOR_H
 
 
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
 
#define High    1
#define Low     0
 
#define IN1(a) if (a)  \
	GPIO_SetBits(GPIOA,GPIO_Pin_2);\
else  \
	GPIO_ResetBits(GPIOA,GPIO_Pin_2)
 
#define IN2(a) if (a)  \
	GPIO_SetBits(GPIOA,GPIO_Pin_3);\
else  \
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
 

void Motor_Config(void); 
void Motor_STOP(void);
void Motor_PRun(void);
void Motor_NRun(void); 
 
#endif
