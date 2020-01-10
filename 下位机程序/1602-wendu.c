#include <reg52.H>
#include <intrins.H>
#include <math.H>
#include <stdio.h>
#include <string.h>

#define uchar unsigned char
#define uint unsigned int
volatile unsigned char sending;
uchar a;

//sbit  = P2^6;
//sbit wela = P2^7;
sbit rw = P1^1; 
sbit RS = P1^0;  
  
sbit LCDEN = P2^5; 

sbit ds = P2^2;
sbit beep = P2^3;
int value = 0;


void delayUs()
{
    _nop_();
}
//延时子程序
 void delayMs(uint a)
{
    uint i, j;
    for(i = a; i > 0; i--)
        for(j = 100; j > 0; j--);
 }

//1602显示
void writeComm(uchar comm)
{
    RS = 0;    
    P0 = comm;
    LCDEN = 1;
    delayUs();
    LCDEN = 0;
    delayMs(1);
}

//写数据:RS=1, RW=0;
void writeData(uchar dat)
{
    RS = 1;
    P0 = dat;
    LCDEN = 1;
    delayUs();
    LCDEN = 0;
    delayMs(1);																	 
}

void init()
{
   rw = 0; 
   //dula = wela = 0;
   writeComm(0x38);
   writeComm(0x0c); 
   writeComm(0x06);
   writeComm(0x01); 
}

void writeString(uchar * str, uchar length)
{
     uchar i;
    for(i = 0; i < length; i++)
    {
         writeData(str[i]);
     }
 }
 
//*****************************DS18B20*******************************//

void dsInit()
 {
    
    unsigned int i;  
    ds = 0;
    i = 100;  
    while(i>0) i--;
    ds = 1;   
    i = 4;
    while(i>0) i--;
 }
 
void dsWait()
 {
      unsigned int i;
      while(ds);  
      while(~ds);
      i = 4;
      while(i > 0) i--;
}


bit readBit()
{
    unsigned int i;
    bit b;
    ds = 0;
    i++;   
    ds = 1; 
    i++; i++;  
    b = ds;
    i = 8; 
    while(i>0) i--;
    return b;
}

unsigned char readByte()
{
    unsigned int i;
    unsigned char j, dat;
    dat = 0;
    for(i=0; i<8; i++)
    {
        j = readBit();
      
        dat = (j << 7) | (dat >> 1);
    }
    return dat;
}


void writeByte(unsigned char dat)
{
    unsigned int i;
    unsigned char j;
    bit b;
    for(j = 0; j < 8; j++)
    {
        b = dat & 0x01;
        dat >>= 1;
    
        if(b)   
        {
           ds = 0;          
		   i++; i++;  
           ds = 1;    
           i = 8; while(i>0) i--;  
        }
        else  
        {
            ds = 0;
          i = 8; while(i>0) i--;  
            ds = 1;
           i++; i++;
        }
   }
}


void sendChangeCmd()
{
    dsInit();    
    dsWait();   
    delayMs(1);    
    writeByte(0xcc);
    writeByte(0x44);
}

void sendReadCmd()
{
    dsInit();
    dsWait();
    delayMs(1);
    writeByte(0xcc); 
    writeByte(0xbe); 
}


int getTmpValue()
{
    unsigned int tmpvalue;
    //int value; 
    float t;
    unsigned char low, high;
    sendReadCmd();
    
    low = readByte(); 
    high = readByte();
   
    tmpvalue = high;
    tmpvalue <<= 8;
    tmpvalue |= low;
    value = tmpvalue;
    
    t = value * 0.0625;

    value = t * 100 + (value > 0 ? 0.5 : -0.5); //大于0加0.5, 小于0减0.5
    return value;
}

void display(int v) 
{
    unsigned char count;
    unsigned char datas[] = {0, 0, 0, 0, 0};
    unsigned int tmp = abs(v);
    datas[0] = tmp / 10000;
    datas[1] = tmp % 10000 / 1000;
    datas[2] = tmp % 1000 / 100;	
    datas[3] = tmp % 100 / 10;
    datas[4] = tmp % 10;
    writeComm(0xc0+3);
    if(v < 0)
    {
        writeString("- ", 2);
    }
    else
    {
       writeString("+ ", 2);
    }
    if(datas[0] != 0)
    {
        writeData('0'+datas[0]);
    }
    for(count = 1; count != 5; count++)
    {
        writeData('0' + datas[count]);
        if(count == 2)
        {
            writeData('.');
        }
    }
}

//串口通信
void cominit(){
	TMOD = 0X20;
	TH1 = 0XFD;
	TL1 = 0XFD;
	TR1 = 1;
	REN = 1;
	SM0 = 0;
	SM1 = 1;
	EA = 1;
	ES = 1;
}
void send(uchar vd[]){
	uint i = 0;
	for(i = 1;i < 6;i++){
		//printf("CHUANKOU:");
		SBUF = vd[i];
		while(!TI);
		TI = 0;
	}
	SBUF = a;
	while(!TI);
	TI = 0;
}
uchar* tostring(int value){
	uchar vd[6] = NULL;
	uint tmp = abs(value);
	sprintf(vd," %d",tmp);
	//itoa(value,vd,10);
	return vd;
}

//报警
void  Waring(int value){
	
	if(value >= 2500){
		beep = 0;
	}
	else if(value <= 1600){
		beep = 0;
	}
	else{
		beep = 1;
	}
}

void main()
{
    uchar table[] = "  xianzaiwendu: ";
    sendChangeCmd();
    init();
	cominit();
    writeComm(0x80);
    writeString(table, 16);
	beep = 1;
    while(1)
    {
        delayMs(1000); //温度转换时间需要750ms以上
        writeComm(0xc0);
        display(getTmpValue());
		Waring(getTmpValue());
		send(tostring(getTmpValue()));
        sendChangeCmd();			
    }
}
