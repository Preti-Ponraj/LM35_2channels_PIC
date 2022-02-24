/*
 * File:   lm35.c
 * Author: Preethi
 *
 * Created on 22 February, 2022, 10:01 PM
 */


#include <xc.h>
#include<stdio.h>
#define  EN RE0
#define  RS RE1
#define  LCD PORTC

    static int a,b,output;
    static char array[16],array1[16];
    static float voltage,temp;
   

void delay(int ms)
{
    int i,j;
    for(i=0;i<ms;i++)
    {
        for(j=0;j<1000;j++);
    }
}
void enable()
{
    EN=1;
    delay(10);
    EN=0;
    delay(10);
}

void command(char a)
{
    RS=0;
    LCD=a;
    enable();
}

void data(char *a)
{
    while(*a)
    {
        RS=1;
        LCD=*a++;
        enable();
    }
}

float adc(char a)
{
    command(0x01);
    ADCON0=a;
    ADCON1=0x80;
    delay(10);
    GO=1;
    while(GO==1);
    a=ADRESL;
    b=ADRESH;
    
    output=a+(b*256);
    
    voltage=(float)output*(4.8); //(5/1023=0.0048V converted to mV)
    temp=voltage/10; //10mV/degreeCelsius
    
    sprintf(array1,"%.2fmv",voltage);
    sprintf(array,"%.2fC",temp);
    
    command(0x80);
    data(array);
    command(0xC0);
    data(array1);
    return temp;
}

void main(void) 
{
    
    TRISA=0x03;
    PORTC=0x00;
    TRISC=0x00;
    PORTE=0x00;
    TRISE=0x00;
    ANSEL=0x03; //AN0 
    PORTD=0x00;
    TRISD=0x00;
    ANSELH=0x00;
    command(0x38);
    //command(0x80);
    command(0x0E);
    
    delay(10);
    while(1)
    {
        float temp;
        
        temp=adc(0x81);
        temp=adc(0x85);
    if(temp>10.00)
    {
        PORTD=0x01;
    }
    else
    {
        PORTD=0x00;
    }
   
}
}