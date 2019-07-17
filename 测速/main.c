#include <reg52.h>

#define uint unsigned int
#define uchar unsigned char

//PWM
sbit PWM = P2^0;
//����
sbit Key1 = P3^4;
sbit Key2 = P3^5;
//LCD
sbit RS = P1^0;
sbit RW = P1^1;
sbit EN = P2^5;

float Kp = 20;Ki = 10;Kd = 0;		//PID�����趨
uint Error = 0,PrevError = 0,LastError = 0; //������
uint SpeedSet = 50;				//�����ٶȳ�ֵ
uint Inpluse = 0,RealSpeed = 0; //����
uint PWM_Time = 50;	            //����������
uint cnt = 0;					//����ռ�ձ�
uint Out = 0;
uchar receive = 0,flag = 0;


/**************��ʱ����*****************/
void delay(uint x)
{
	uint i,j;
	for(i = x;i > 0;i--)
		for(j = 110;j > 0;j--);
}
/**************PID���*****************/
 void PID_Control()
{
    //����ƫ��
	//e(k)= r(k) - c(k)
	Error = SpeedSet - RealSpeed;
	//����ʽPID����
	//��u(k) = Kp*(e(k)-e(k-1))+Ki*e(k)+Kd*(e(k)-2*e(k-1)+e(k-2))+U0
	Out = Out + ((Kp * (Error - LastError) + Ki * Error + Kd * (Error + PrevError - 2 * LastError))/50);
	//������
	//e(k-1)��e(k-2)   e(k)��e(k-1)
	PrevError = LastError;
	LastError = Error;
	//
	PWM_Time = Out ;
}
/**************����ɨ��*****************/
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
/**************�ж�����*****************/
//���ö�ʱ��T0��T1
//��ʱ��0��������ʽ2����װ��ֵ��8Ϊ��ʱ��
//��ʱ��1��������ʽ1��16λ��ʱ��
void Init()
{
	TMOD = 0x21;			   //0010 0001
	TH0 = (65536 - 2000)/256;
	TL0 = (65536 - 2000)%256;  //2ms
	TH1 = (65536 - 100)/256;
	TL1 = (65536 - 100)%256;   //0.1ms
	TR0 = 1;				   //����T0
	ET0 = 1;				   //T0�ж�����
	TR1 = 1;				   //����T1
	ET1 = 1;				   //T1�ж�����
	EX0 = 1;				   //INT0�ж�����
	IT0 = 1;				   //INT0Ϊ���ش���
	EA = 1;					   //�����ж�
}
//�ⲿ�ж�0 ��¼������ ���ڲ���
void INT_0() interrupt 0
{
	Inpluse++;					//INT0�������ڲ���
}
//��ʱ��T0��ÿ��ʱ2ms time += 1��
//��time = 100����ʱ��ʱ50*2 =100ms 
void T0_Time()	 interrupt 1
{
	static uchar time = 0;
	TR0 = 0;				   //�ر�T0
	TH0 = (65536 - 2000)/256;  //��װ��ֵ
	TL0 = (65536 - 2000)%256;	   
	TR0 = 1;				   //��T0
	time++;					   //����+1
	if(time == 50)			   //200ms
	{
	    time = 0;
		flag = 1;
		RealSpeed = (Inpluse * 10) / 2;   //�������ʱ���ٶ� Inpluse r/s
		Inpluse = 0;		   //��ʼ����INT0���¼�¼������
		PID_Control();		   //���ٶȽ���PID����
	}
}
void T1_Time()	interrupt 3
{
	cnt++;                     //ÿ0.1ms +1 �����ڿ���ռ�ձ�
}
/**************�������****************/
void PWM_Out()
{
	if(cnt < PWM_Time)		   //PWM_Time=50,��ʱռ�ձ�Ϊ50%
	{
		PWM = 1;
	}
	else
		PWM = 0;
	if(cnt > 100)		       //cnt�趨���� ��ʱ�ź�����Ϊ10ms
	{
		cnt = 0;
	}
}

/*************�ж�æµ*****************/
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
/*************д������*****************/
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
/*************��������*****************/
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
/*************��ʾָ��*****************/
void Write_String(uchar *s)
{
	while(*s)
	{
		Write_Date(*s);
		s++;
	}
}
/*************LCD��ʼ��*****************/
void LCD_Init()
{
	RS = 1;
	RW = 1;
	EN = 0;
	Write_Com(0x38);//����16*2��ʾ��5*7����8λ���ݽӿ�
	delay(1);
	Write_Com(0x01);//��ʾ���㣬����ָ������
	delay(1);
	Write_Com(0x0c);//���ÿ���ʾ������ʾ���
	delay(1);
	Write_Com(0x06);//дһ���ַ����ַ+1
}
/**************������******************/
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