#include "sys.h"
#include "motor.h"  
#include "stm32f10x.h"  
#include "stm32f10x_rcc.h"  
#include "delay.h"      
#include "stm32f10x_tim.h"
void Motor_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;   
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_2|GPIO_Pin_3); 
}
 
void Motor_STOP(void){IN1(High);IN2(High);} 
void Motor_PRun(void){IN1(Low);IN2(High);}
void Motor_NRun(void){IN1(High);IN2(Low);}
