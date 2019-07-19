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
	Motor_Config();      //电机函数初始化
	delay_init();	    	 //延时函数初始化	
	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
	uart_init(115200);	 //串口初始化 115200
	NVIC_Configuration();//设置NVIC中断分组2:2位抢占优先级，2位响应优先级	 
	//PWM频率= 72000000/900=80Khz
	TIM3_PWM_Init(899,0);
	pwmval = 0;
	while(1)
	{
		u8 cnm = 0;
 		t = KEY_Scan(1);		//键盘扫描
		if(t == 1 )
		{	//停止
			Motor_STOP();
		}
		if(t == 2)
		{//正反转
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
		{//加速	
			pwmval = 300;
			TIM_SetCompare2(TIM3,pwmval);
		}
		if(t == 4)
		{//减速
			pwmval = 0;
			TIM_SetCompare2(TIM3,pwmval);
		}
	}
}

						
