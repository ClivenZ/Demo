#include "key.h"
#include "stm32f10x.h"
#include "delay.h"


void KEY_Init(void){
  GPIO_InitTypeDef GPIO_InitStructure;
	//初始化KEY0-->GPIOA.13,KEY1-->GPIOA.15  上拉输入
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4 | GPIO_Pin_1 | GPIO_Pin_5;//PE2~4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE1,2,3,4

//	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;//下拉输出
//	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
//	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                  
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                 
//	GPIO_Init(GPIOB, &GPIO_InitStructure);                                     
//	GPIO_SetBits(GPIOB,GPIO_Pin_5);                                              
}

//按键处理函数
//返回按键值
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!

u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||KEY3==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)return KEY_S1;
		else if(KEY1==0)return KEY_S2;
		else if(KEY2==0)return KEY_S3;
		else if(KEY3==0)return KEY_S4;
	}else if(KEY0==1&&KEY1==1&&KEY2==1&&KEY3==1)key_up=1; 	    
		return 0;// 无按键按下
}
