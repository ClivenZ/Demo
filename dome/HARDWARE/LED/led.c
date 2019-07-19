#include "led.h"
#include "stm32f10x.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//GPIOB使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
////	//GPIOE使能
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);

	//设置推挽输出
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	//选择IO5
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	//设置速度
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//PB5 默认高电平
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
//	
	
	
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOE,&GPIO_InitStructure);
//	GPIO_SetBits(GPIOE,GPIO_Pin_5);
}



