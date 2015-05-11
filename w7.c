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

float pwm_freq=20.0;//KHz
char pwm_duty=25;//%25


void main() {
    int dum;
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

    while (1){
    }
}
