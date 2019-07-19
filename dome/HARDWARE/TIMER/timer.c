#include "timer.h"
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "led.h"

////��ʱ��3��ʼ��
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

//	//ʹ�ܶ�ʱ��3�ĸ����ж�
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
//	
//	//�ж����ȼ�NVIC����
//	NVIC_InitStruct.NVIC_IRQChannel=TIM3_IRQn; //TIM3�ж�
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;//ǿռ���ȼ�0
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;//�����ȼ�3
//	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//IRQͨ��ʱ��
//	NVIC_Init(&NVIC_InitStruct);//��ʼ��NVIC�Ĵ���
//	
//	
//	//ʹ�ܶ�ʱ��
//	TIM_Cmd(TIM3,ENABLE);

//}

////�жϷ�����
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
//	//GPIOBʹ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	//PB5 Ĭ�ϸߵ�ƽ
//	GPIO_SetBits(GPIOB,GPIO_Pin_5);

//	//ʹ�ܶ�ʱ��3ʱ��
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
//	//ʹ��GPIOB�����AFIO���ù���ģ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
//	//��ʱ��3�Ĳ�����ӳ��
//	//ͨ��2 PB5 
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);

//	
//	//��ʱ��3��ʼ��
//	TIM_TimeBaseInitStruct.TIM_Period=arr;
//	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
//	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
//	
//	
//	//PWMģʽ2
//	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM2;
//	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
//	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
//	//��ʼ������Ƚϲ���
//	TIM_OC2Init(TIM3,&TIM_OCInitStruct);
//	
//	//Ԥװ��
//	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
//	//��ʱ��ʹ��
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
        
	 //ʹ�ܶ�ʱ��3ʱ��
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	 //ʹ��GPIO������
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //??GPIO???AFIO????????
   //TIM3������ӳ��     
   GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);   

   //���������TIM3 CH2 -> PWM        GPIOB.5
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //��������
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
        
   //��ʼ��TIM3
   TIM_TimeBaseStructure.TIM_Period = arr; 
   TIM_TimeBaseStructure.TIM_Prescaler =psc; 
   TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���
   TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	 
   //��ʼ��TIM3 channel2 -> PWM mode       
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

	 //ʹ��TIM3
   TIM_Cmd(TIM3, ENABLE);
        

}
