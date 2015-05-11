/*
 * File:   week6_1.c
 * Author: alperssisman
 *
 * Created on 23 Nisan 2014 Çar?amba, 22:32
 */

#include <pic18f452.h>
#include <htc.h>
#include "delay.h"
#define _XTAL_FREQ 8000000

#pragma config OSCS = OFF, OSC=HS, BOR = OFF, PWRT=OFF,WDT=OFF,CCP2MUX=OFF,LVP=OFF,STVR=OFF

int counter=0;

void interrupt Timer0_ISR(void)
{
    if (T0IF) //are TMR0 interrupts enabled and
    //is the TMR0 interrupt flag set?
    {
        T0IF=0; //TMR0 interrupt flag must be cleared in software
        //to allow subsequent interrupts
        counter++; //increment the counter variable by 1
        if (counter==5000){
            counter=0;
        }
        TMR0 = 256-125;
    }
}

void main() {
    ADCON0=0x00;
    ADCON1=0x07;
    // timer interrupt init
    TMR0 = 256-125;
    T08BIT=1;
    T0CS=0;
    PSA=1;
    T0PS0=1;
    T0PS1=1;
    T0PS2=0;
    TMR0ON=1;
    TMR0IE = 1;
    GIE=1;
    
    TRISE=0x00;

    while (1){
        if (!counter){
            RE0 = ~RE0;
        }
    }
}
