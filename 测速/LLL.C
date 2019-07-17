#include <reg52.h>
#include <stdio.h>
sbit plus_1=P1^1;				  //定义继电器对应单片机管脚
sbit minus_1=P1^2;
sbit plus_10=P1^3;
sbit minus_10=P1^4;
sbit plus_20=P1^5;

sbit PWM=P2^5;            //PWM输出
//LCD//
sbit RS=P2^0;
sbit RW=P2^1;
sbit EN=P2^2;

//motor//
unsigned int tt0;		 //T0中断
unsigned int tt1;		 //T1中断

unsigned char flag;		//抽样时间
unsigned int speed=100;	
unsigned int set=50;	//设定速度
unsigned int a;
unsigned char receive;
unsigned int ReData,sum=0,count=0;
float Kp,Ki,Kd,Ti,Td,T;		//	pid参数定义
int ek,ek1,ek2,ekx;									
double Uk;		

/****************************函数定义***********************************/
//void delay(unsigned intxms)                  //延时
//bit lcd_busy()				                 //判断LCD1602是否忙碌          
//void write_com(unsigned char com)		     //LCD1602写命令函数
//void write_date(unsigned char date)		     //LCD1602写数据函数
//void write_string(unsigned char *s)     	//液晶LCD1602显示指针
//void lcd_initial()					//LCD1602初始化
//void keyscan()                             //设置速度键盘扫描
//void init()			                       //定时器初始化
//void t_0() interrupt 1			          //定时
//void t_1() interrupt 3			//驱动电机
//void t_2()	interrupt 5		            //计数器
//void pid(unsigned int count)            //增量PID控制

 /***********************************延时********************/
void delay(unsigned int x)  

{
unsigned i, j;
	for(i = x; i>0; i--)
		for(j = 110; j >0; j--);
}

bit lcd_busy()				//判断LCD1602是否忙碌			
{
	bit result;
	P0=0xff;						
	RS=0;
	RW=1;
    _nop_();
    _nop_();
    _nop_();
	_nop_();
	delay(1);
	EN=1;
	delay(1);
	result=(bit)(P0&0x80);			
	EN=0;
	return result;
}
void write_com(unsigned char com)		//LCD1602写命令函数
{
	while(lcd_busy());                                                          
	RS=0;
	RW=0;
	P0=com;
 _nop_();
   _nop_();
_nop_();
	_nop_();
	delay(1);				
	EN=1;
	delay(1);				
	EN=0;
}
                                                                          
void write_date(unsigned char date)				//LCD1602写数据函数
{
	while(lcd_busy());
	RS=1;
	RW=0;
	P0=date;
 _nop_();
   _nop_();
_nop_();
	_nop_();
	delay(1);
	EN=1;
	delay(1);
	EN=0;
}

void write_string(unsigned char *s) 	//液晶LCD1602显示指针
 {     

while (*s) 
	{     
write_date(*s);     
s++;
	}
 }
void lcd_initial()					//LCD1602初始化
{
	RS=1;
	RW=1;
	EN=0;                                                                  
 _nop_();
   _nop_();
_nop_();
	_nop_();
	write_com(0x38);	//设置16X2显示，5X7点阵，8位数据接口
	delay(1);		
	write_com(0x01);	//显示清零，数据指针清零
	delay(1);			
	write_com(0x0c);	//设置开显示，不显示光标
	delay(1);			
	write_com(0x06);	//写一个字符后地址指针加1								
}
/*****************   设置速度键盘扫描    **************************/
voidkeyscan()						
{
	if(plus_1==0)
	{
	delay(10);
	if(plus_1==0)
	   {
	set=set+1;
		if(set>=50)set=50;
		while(!plus_1);
		receive=1;
	   }
	}
	if(minus_1==0)
	{
	delay(10);
	if(minus_1==0)
	   {
	set=set-1;
		if(set<=20)set=20;
		while(!minus_1);
		receive=1;
	   }
	}
	if(plus_10==0)
	{
	delay(10);
	if(plus_10==0)
	   {
	set=set+10;                                                              
		if(set>=50)set=50;
		while(!plus_10);
		receive=1;
	   }
	}
	if(minus_10==0)
	{
	delay(10);
	if(minus_10==0)
	   {
	set=set-10;
		if(set<=20)set=20;
		while(!minus_10);
		receive=1;
	   }
	}
	if(plus_20==0)
	{
	delay(10);
	if(plus_20==0)
	   {
	set=set+20;
		 if(set>=50)set=50;
		while(!plus_20);
		receive=1;
	   }
	}
}
void init()			 //定时器初始化
{
	TMOD=0x23;					//0010_0011 定时器0:模式3		定时器1:模式2
	SCON=0x50;                 //0101_0000  串行口方式1  10位UART 波特率可变
	PCON|=0x80;					//SMOD=1    串行口波特率加倍
	T2CON=0x02;					//计数器2	仅当定时器溢出时捕获和重装
	TH0=(256-100);						//驱动电机
	TL0=(256-250);				//定时0.25ms
	TH1=243;					//256-2^1*11.0592/(384*9600)		波特率为4800
	TH2=0;
	TL2=0;
	TR0=1;          //启动定时器
	TR1=1;
	TR2=1;
	ET0=1;          //允许T0中断                                            
	ET1=1;
	ET2=1;
	ES=1;         //串行口中断位
	EA=1;        //中断允许位
	
}
//************************************************************************ //
void t_0() interrupt 1			//定时100ms(抽样时间)
{   
	TL0=(256-250);
	tt0++;
	if(tt0==410)
	{
		tt0=0;	
	flag=1;
		
	}
}
//************************************************************************
void t_1() interrupt 3			//驱动电机	PWM		  speed*发生一次中断时间=低电平，（100-speed）*中断时间=高电平
{
	TH0=(256-100);
	tt1++;
	if(tt1>=250)
		tt1=0;
	if(tt1<speed)	PWM=1;				   //speed是控制占空比的	
	else PWM=0;
	
}

void t_2()	interrupt 5		//计数器
{
	TH2=0;
	TL2=0;
	TF2=0;	
}
									
/****************增量式PID控制******************************/												
voidpid(unsigned int count)				
{													
	ek=set-count; 	            //偏差值								
	if(ek>=5&&ek<=-5)			//积分分离                                  
    {Uk=Kp*(ek-ek1)+Ki*ek+Kd*(ek-2*ek1+ek2);}	  //增量PID 			
	else Uk=Kp*ek;	
	speed=speed+Uk;
	if(speed>300)speed=300;		//抗积分饱和
	if(speed<0)  speed=0;	
	ek2=ek1;
	ek1=ek;
}

/**************************主函数*********************************************/

void main()
{

unsigned char ww=1;	
	T=1;
	Kp=4.5;
	Ti=0.8;
	Td=0;

	Ki=Kp*T/Ti;
	Kd=Kp*Td;
	keyscan();
	lcd_initial();
	write_string(" set:     r/s");
	write_com(0x80+0x40);          //第二行
	write_string("real:     r/s");
	write_com(0x80+6);
	write_date(set/100+0x30);			  //设置的数据
	write_date(set%100/10+0x30);		
	write_date(set%10+0x30);
	init();	
	while(1)
	{
	keyscan();				   //LCD1602显示设置的速度值
		if(receive)											
		{
			receive=0;
			
			write_com(0x80+6);
			write_date(set/100+0x30);		
			write_date(set%100/10+0x30);			
			write_date(set%10+0x30);
		}	     
		if(flag)                        //100ms
		{
			unsigned char num;		 //LCD1602显示真实的速度值
			
			flag=0;
		
			a=(TH2*256+TL2);	  //r/s
		    TH2=0;
		    TL2=0;			
			num++;
			sum=sum+a;		//累加10次达到一秒
			if(num==10)
			{
				count=sum;
				num=0;
				sum=0;
				count=count/448;
				write_com(0x80+0x40+6);
			write_date(count/100+0x30);		
	        write_date(count%100/10+0x30);			
			write_date(count%10+0x30);	
			if(ww==1)
			   {
			TI=1;
				printf("%d",0);
				while(!TI);
				TI=0;
				delay(10);
				ww=ww-1;	
			   }
			else
				{
				TI=1;
				printf("%d",count);
				while(!TI);
				TI=0;
				delay(10);
				}				
			pid(count);
		   }
		}
	}
	}