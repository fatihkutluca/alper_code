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

int main() {
    ADCON1=0x07;
    TRISD=0X00;
    DelayMs(250);
    LCD_INIT();
    DelayMs(200);
    while(1){
        CMD(0x80);
        string1("Hello World");
        CMD(0xC0);
        string1("CALISIYOR!!!");
        while(1);
    }
}
