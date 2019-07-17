#include <reg52.h>

#define uint unsigned int
#define uchar unsigned char

//PWM
sbit PWM = P2^0;
//按键
sbit Key1 = P3^4;
sbit Key2 = P3^5;
//LCD
sbit RS = P1^0;
sbit RW = P1^1;
sbit EN = P2^5;

float Kp = 20;Ki = 10;Kd = 0;		//PID参数设定
uint Error = 0,PrevError = 0,LastError = 0; //误差参数
uint SpeedSet = 50;				//设置速度初值
uint Inpluse = 0,RealSpeed = 0; //测速
uint PWM_Time = 50;	            //设置脉冲宽度
uint cnt = 0;					//调节占空比
uint Out = 0;
uchar receive = 0,flag = 0;


/**************延时函数*****************/
void delay(uint x)
{
	uint i,j;
	for(i = x;i > 0;i--)
		for(j = 110;j > 0;j--);
}
/**************PID输出*****************/
 void PID_Control()
{
    //计算偏差
	//e(k)= r(k) - c(k)
	Error = SpeedSet - RealSpeed;
	//增量式PID计算
	//▲u(k) = Kp*(e(k)-e(k-1))+Ki*e(k)+Kd*(e(k)-2*e(k-1)+e(k-2))+U0
	Out = Out + ((Kp * (Error - LastError) + Ki * Error + Kd * (Error + PrevError - 2 * LastError))/50);
	//误差更新
	//e(k-1)→e(k-2)   e(k)→e(k-1)
	PrevError = LastError;
	LastError = Error;
	//
	PWM_Time = Out ;
}
/**************键盘扫描*****************/
void KeyScan()
{
	if(Key1 == 0)
		delay(5);
		if(Key1 == 0)
		{
			SpeedSet += 50;
			receive = 1;
			if(SpeedSet > 3000)
			{
				SpeedSet = 3000;
				receive = 1;
			}
		}
	 if(Key2 == 0)
	 	delay(5);
		if(Key2 == 0)
		{
			SpeedSet -= 50;
			receive = 1;
			if(SpeedSet == 0)
				SpeedSet = 0;
				receive = 1;		
		}
}
/**************中断设置*****************/
//设置定时器T0、T1
//定时器0：工作方式2：重装初值的8为定时器
//定时器1：工作方式1：16位定时器
void Init()
{
	TMOD = 0x21;			   //0010 0001
	TH0 = (65536 - 2000)/256;
	TL0 = (65536 - 2000)%256;  //2ms
	TH1 = (65536 - 100)/256;
	TL1 = (65536 - 100)%256;   //0.1ms
	TR0 = 1;				   //启动T0
	ET0 = 1;				   //T0中断允许
	TR1 = 1;				   //启动T1
	ET1 = 1;				   //T1中断允许
	EX0 = 1;				   //INT0中断允许
	IT0 = 1;				   //INT0为边沿触发
	EA = 1;					   //打开总中断
}
//外部中断0 记录脉冲数 用于测速
void INT_0() interrupt 0
{
	Inpluse++;					//INT0计数用于测速
}
//定时器T0：每定时2ms time += 1；
//当time = 100，此时定时50*2 =100ms 
void T0_Time()	 interrupt 1
{
	static uchar time = 0;
	TR0 = 0;				   //关闭T0
	TH0 = (65536 - 2000)/256;  //重装初值
	TL0 = (65536 - 2000)%256;	   
	TR0 = 1;				   //打开T0
	time++;					   //计数+1
	if(time == 50)			   //200ms
	{
	    time = 0;
		flag = 1;
		RealSpeed = (Inpluse * 10) / 2;   //计算出此时的速度 Inpluse r/s
		Inpluse = 0;		   //初始化，INT0重新记录脉冲数
		PID_Control();		   //将速度进行PID调整
	}
}
void T1_Time()	interrupt 3
{
	cnt++;                     //每0.1ms +1 ，用于控制占空比
}
/**************脉冲输出****************/
void PWM_Out()
{
	if(cnt < PWM_Time)		   //PWM_Time=50,此时占空比为50%
	{
		PWM = 1;
	}
	else
		PWM = 0;
	if(cnt > 100)		       //cnt设定周期 此时信号周期为10ms
	{
		cnt = 0;
	}
}

/*************判断忙碌*****************/
bit LCD_Busy()
{
	bit result;
	P0 = 0xff;
	RS = 0;
	RW = 1;
	delay(1);
	EN = 1;
	delay(1);
	result = (bit)(P0&0x80);
	EN = 0;
	return result;
}
/*************写入命令*****************/
void Write_Com(uchar com)
{
	while(LCD_Busy());
	RS = 0;
	RW = 0;
	P0 = com;
	delay(1);
	EN = 1;
	delay(1);
	EN = 0;
}
/*************读入命令*****************/
void Write_Date(uchar date)
{
	while(LCD_Busy());
	RS = 1;
	RW = 0;
	P0 = date;
	delay(1);
	EN = 1;
	delay(1);
	EN = 0;
}
/*************显示指针*****************/
void Write_String(uchar *s)
{
	while(*s)
	{
		Write_Date(*s);
		s++;
	}
}
/*************LCD初始化*****************/
void LCD_Init()
{
	RS = 1;
	RW = 1;
	EN = 0;
	Write_Com(0x38);//设置16*2显示，5*7点阵，8位数据接口
	delay(1);
	Write_Com(0x01);//显示清零，数据指针清零
	delay(1);
	Write_Com(0x0c);//设置开显示，不显示光标
	delay(1);
	Write_Com(0x06);//写一个字符后地址+1
}
/**************主函数******************/
void main()
{
	 LCD_Init();
	 Write_String("set:       r/s");
	 Write_Com(0x80 + 0x40);
	 Write_String("real:      r/s");
	 Write_Com(0x80 + 6);
//   Write_Date(SpeedSet/1000 + 0x30);
//	 Write_Date(SpeedSet%1000/100 + 0x30);
//	 Write_Date(SpeedSet%100/10 + 0x30);
//	 Write_Date(SpeedSet%10 + 0x30);
	 Init();
	 while(1)
	 {
	 	KeyScan();
		if(receive == 1)											
		{
			receive = 0;			
			Write_Com(0x80 + 6);
			Write_Date(SpeedSet/1000 + 0x30);
			Write_Date(SpeedSet%1000/100 + 0x30);		
			Write_Date(SpeedSet%100/10 + 0x30);			
			Write_Date(SpeedSet%10 + 0x30);
		}
		if(flag == 1)
		{
			Write_Com(0x80+0x40+6);
			Write_Date(RealSpeed/1000 + 0x30);		
	    	Write_Date(RealSpeed%1000/100 + 0x30);			
			Write_Date(RealSpeed%100/10 + 0x30);
			Write_Date(RealSpeed%10 + 0x30);	
		}	            
	 	PWM_Out();	 	 
     }	  
}