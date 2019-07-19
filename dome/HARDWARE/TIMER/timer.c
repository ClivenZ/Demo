#include "timer.h"
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "led.h"

////定时器3初始化
//void TIM3_Int_Init(u16 arr,u16 psc){
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//	NVIC_InitTypeDef NVIC_InitStruct;
//	TIM_TypeDef TIM_TypeStruct;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
//	
//	TIM_TimeBaseInitStruct.TIM_Period=arr;
//	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
//	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);

//	//使能定时器3的更新中断
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
//	
//	//中断优先级NVIC设置
//	NVIC_InitStruct.NVIC_IRQChannel=TIM3_IRQn; //TIM3中断
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;//强占优先级0
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;//从优先级3
//	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//IRQ通道时能
//	NVIC_Init(&NVIC_InitStruct);//初始化NVIC寄存器
//	
//	
//	//使能定时器
//	TIM_Cmd(TIM3,ENABLE);

//}

////中断服务函数
//void TIM3_IRQHandler(){
//	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET){
//		LED1 = !LED1;
//		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
//	}
//}

//void TIM3_PWM_Init(u16 arr,u16 psc){
//		GPIO_InitTypeDef GPIO_InitStructure;
//		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//		TIM_OCInitTypeDef TIM_OCInitStruct;
//	//GPIOB使能
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	//PB5 默认高电平
//	GPIO_SetBits(GPIOB,GPIO_Pin_5);

//	//使能定时器3时钟
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
//	//使能GPIOB外设和AFIO复用功能模块
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
//	//定时器3的部分重映射
//	//通道2 PB5 
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);

//	
//	//定时器3初始化
//	TIM_TimeBaseInitStruct.TIM_Period=arr;
//	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
//	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
//	
//	
//	//PWM模式2
//	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM2;
//	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
//	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
//	//初始化输出比较参数
//	TIM_OC2Init(TIM3,&TIM_OCInitStruct);
//	
//	//预装载
//	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
//	//定时器使能
//	TIM_Cmd(TIM3,ENABLE);
//	
//	
//	
//}


void TIM3_PWM_Init(u16 arr,u16 psc)
{  
   GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;
        
	 //使能定时器3时钟
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	 //使用GPIO口外设
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //??GPIO???AFIO????????
   //TIM3部分重映射     
   GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);   

   //复用输出，TIM3 CH2 -> PWM        GPIOB.5
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
        
   //初始化TIM3
   TIM_TimeBaseStructure.TIM_Period = arr; 
   TIM_TimeBaseStructure.TIM_Prescaler =psc; 
   TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数
   TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	 
   //初始化TIM3 channel2 -> PWM mode       
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
   TIM_OC2Init(TIM3, &TIM_OCInitStructure);  
   TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 
        
           
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
   TIM_OC3Init(TIM3, &TIM_OCInitStructure);  
   TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	 //使能TIM3
   TIM_Cmd(TIM3, ENABLE);
        

}
