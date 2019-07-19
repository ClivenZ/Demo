#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "sys.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "timer.h"
#include "usart.h"
#include "motor.h"

int main(void)
 {
 	u8 t;
	u8 dir = 0;
	u16 pwmval;
	Motor_Config();      //���������ʼ��
	delay_init();	    	 //��ʱ������ʼ��	
	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	uart_init(115200);	 //���ڳ�ʼ�� 115200
	NVIC_Configuration();//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�	 
	//PWMƵ��= 72000000/900=80Khz
	TIM3_PWM_Init(899,0);
	pwmval = 0;
	while(1)
	{
		u8 cnm = 0;
 		t = KEY_Scan(1);		//����ɨ��
		if(t == 1 )
		{	//ֹͣ
			Motor_STOP();
		}
		if(t == 2)
		{//����ת
			while(dir <= 200){
//				TIM_SetCompare2(TIM3,pwmval);
//				Motor_PRun();
//				delay_ms(20000);
//				Motor_NRun();
//				delay_ms(20000);
				cnm = KEY_Scan(1);
				pwmval = 150;
				TIM_SetCompare2(TIM2,pwmval);	
			if(cnm == 1)
				break;
			else					
				dir++;
			delay_ms(10);
//			TIM_SetCompare2(TIM2,pwmval);
//			TIM_SetCompare3(TIM3,pwmval);
				Motor_PRun();
				delay_ms(20000);
				Motor_NRun();
				delay_ms(20000);
			}
		}
		if(t == 3)
		{//����	
			pwmval = 300;
			TIM_SetCompare2(TIM3,pwmval);
		}
		if(t == 4)
		{//����
			pwmval = 0;
			TIM_SetCompare2(TIM3,pwmval);
		}
	}
}

						
