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

#define row1 RB7
#define row2 RB6
#define row3 RB5
#define row4 RB4

#define col1 RB3
#define col2 RB2
#define col3 RB1
#define col4 RB0

#define BUTTON RD6

#define RS RD0
#define EN RD1

char key;

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
char rowscan(){
    char pressed_key=0;
    if (row1){
        DelayMs(100);
        if (row1)
            pressed_key=1;
    }
    if (row2){
        DelayMs(100);
        if (row2)
            pressed_key=5;
    }
    if (row3){
        DelayMs(100);
        if (row3)
            pressed_key=9;
    }
    if (row4){
        DelayMs(100);
        if (row4)
            pressed_key=13;
        }
    return pressed_key;
}

////////////////////////////////////////////////

int main() {
    char LCD_codes[17]={'0','1', 0x32, '3', 'A', '4', '5', '6', 'B','7', '8', '9','C','*','0','#','D'};
    ADCON0=0x00;
    ADCON1=0x07;

    TRISB=0xF0;//b'11110000';
    TRISD=0X40;//b'01000000';
    DelayMs(250);
    LCD_INIT();
    DelayMs(200);
    while(1){
        col1=1;
        col2=0;
        col3=0;
        col4=0;
        key = rowscan();
        if (key!=0){
            DATA(LCD_codes[key]);
        }

        col1=0;
        col2=1;
        col3=0;
        col4=0;
        key = rowscan();
        if (key!=0){
            DATA(LCD_codes[key+1]);
        }
        col1=0;
        col2=0;
        col3=1;
        col4=0;
        key = rowscan();
        if (key!=0){
            DATA(LCD_codes[key+2]);
        }

        col1=0;
        col2=0;
        col3=0;
        col4=1;
        key = rowscan();
        if (key!=0){
            DATA(LCD_codes[key+3]);
        }
        if(!BUTTON){
            DelayMs(100);
            if(!BUTTON)
                CLR();
        }
    }
}
