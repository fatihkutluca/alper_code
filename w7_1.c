/*
 * File:   week4_1.c
 * Author: alperssisman
 *
 * Created on 03 Nisan 2014 Per?embe, 01:26
 */

#include <pic18f452.h>
#include <htc.h>
#include "delay.h"
#define _XTAL_FREQ 8000000
#pragma config OSCS = OFF, OSC=HS, BOR = OFF, PWRT=OFF,WDT=OFF,CCP2MUX=OFF,LVP=OFF,STVR=OFF


#define RS RD0
#define EN RD1
#define TMR2_prescaler 1//x4 or x16
#define AVG 100 //Max=65000/100=650

float pwm_freq=5.0;
char pwm_duty=50,avg[AVG];
int ADC_value,voltage;

// LCD ROUTINES//
void LCD_STROBE(void){
   EN=1;
   DelayUs(1);
   EN=0;
}

void DATA(unsigned char c){
   RS=1;
   DelayUs(50);
   PORTD = PORTD & 0xC3;
   PORTD = PORTD | ((c&0xF0)>>2);
   LCD_STROBE();
   PORTD = PORTD & 0xC3;
   PORTD = PORTD | ((c&0x0F)<<2);
   LCD_STROBE();
}

void CMD(unsigned char c){
   RS=0;
   DelayUs(50);
   PORTD = PORTD & 0xC3;
   PORTD = PORTD | ((c&0xF0)>>2);
   LCD_STROBE();
   PORTD = PORTD & 0xC3;
   PORTD = PORTD | ((c&0x0F)<<2);
   LCD_STROBE();
}

void CLR(void){
   CMD(0x01);
   DelayMs(2);
}

void LCD_INIT(void){
   DelayMs(20);
   CMD(0x38);
   DelayMs(6);
   CMD(0x38);
   DelayUs(120);
   CMD(0x38);
   CMD(0x28);
   CMD(0x28);
   CMD(0x0C);
   CLR();
   CMD(0x06);
}

void string1 (char *q){
   while (*q){
      DATA(*q++);
   }
}

////////////////////////////////////////////////
char int2str(int q){
    char strr[6]="00000\0",i;
    int onuzeri[4]={1,10,100,1000};
    for (i=0;i<5;i++){
        strr[4-i]=0x30+(((int)(q/onuzeri[i]))%10);

    }
    return strr;
}

void main() {
    int dum,sum;
    char i=0,k;

    // pwm init
    PR2=(char)(((_XTAL_FREQ/pwm_freq)/TMR2_prescaler)/4000)-1;
    dum=(int)(((pwm_duty/pwm_freq)*_XTAL_FREQ)/(100000*TMR2_prescaler));
    CCPR1L = dum>>2;
    CCP1CON &= 0b11001111;
    CCP1CON |= (dum & 0x0003)<<4;
    TRISC = 0x00;
    T2CKPS0=0;T2CKPS1=0;

    if(TMR2_prescaler==4){
        T2CKPS0=1;T2CKPS1=0;
    }
    else if(TMR2_prescaler==16){
        T2CKPS0=1;T2CKPS1=1;
    }

    TMR2ON = 1;
    CCP1M3=1;
    CCP1M2=1;
    CCP1M1=1;
    CCP1M0=1;
    ADCON1=0x84;
    ADCON0=0x00;

    TRISA=0X0F;
    TRISD=0x00;

    DelayMs(250);
    LCD_INIT();
    DelayMs(200);
    CMD(0x80);
    string1("Frekans: 5 KHz");
    CMD(0xC0);
    string1("Cal. Or.:");

    while (1){
        ADON=1;
        DelayUs(100);
        GO_DONE=1;
        while(GO_DONE);
        ADC_value=ADRESL+256*ADRESH;
        ADON=0;
        
        pwm_duty=(char)(ADC_value/10.24);
        sum=pwm_duty;
        //avg[i%AVG]=pwm_duty;

        //sum=0;
        //for (k=0;k<AVG;k++)
        //    sum+=avg[k];
        //sum = (int)(sum / AVG);
        
        dum=(int)(((sum/pwm_freq)*_XTAL_FREQ)/(100000*TMR2_prescaler));
        CCPR1L = dum>>2;
        CCP1CON &= 0b11001111;
        CCP1CON |= (dum & 0x0003)<<4;

        CMD(0xCA);
        string1(int2str(sum));
    }
}
