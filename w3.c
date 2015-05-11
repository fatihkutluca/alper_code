/*
 * File:   week3.c
 * Author: alperssisman
 *
 * Created on 24 Mart 2014 Pazartesi, 17:37
 */

#include "delay.h"
#include <pic18f452.h>
#include <htc.h>
#define _XTAL_FREQ 8000000
#define COM2 LATAbits.LATA5
#define COM1 LATAbits.LATA4

// #define sega RA3
// #define segb RA2
// #define segc RC1
// #define segd RC2
// #define sege RC3
// #define segf RC4
// #define segg RC5

#define row1 RB7
#define row2 RB6
#define row3 RB5
#define row4 RB4

#define col1 RB3
#define col2 RB2
#define col3 RB1
#define col4 RB0

#pragma config OSCS = OFF, OSC=HS, BOR = OFF, PWRT=OFF,WDT=OFF,CCP2MUX=OFF,LVP=OFF,STVR=OFF
char key;
void sevseg(char number){

    //                                     1      2   3     A    4     5     6       B     7      8     9     C   *(E)   0    #(F)    D
    char sevenseg_codes_portc[17]={0x00, 0x02, 0x2C, 0x26, 0x3B, 0x32, 0x36, 0x3E, 0x3E, 0x02,  0x3E, 0x36, 0x1C, 0x3C, 0x1E, 0x38, 0x2E};
    char sevenseg_codes_porta[17]={0x00, 0x04, 0x0C, 0x0C, 0x0C, 0x04, 0x08, 0x08, 0x00, 0x0C,  0x0C, 0x0C, 0x08, 0x08, 0x0C, 0x08, 0x04};
    PORTC=sevenseg_codes_portc[number];
    PORTA=sevenseg_codes_porta[number];
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

int main() {

    ADCON1=0x07;
    ADCON0=0x00;
    TRISB=0xF0;//b'11110000';
    TRISC=0xC1;//b'11000001';
    TRISA=0xC3;//b'11000011'

    while(1){
        COM1=1;
        COM2=1;

        col1=1;
        col2=0;
        col3=0;
        col4=0;
        key = rowscan();
        if (key!=0){
            sevseg(key);
        }

        col1=0;
        col2=1;
        col3=0;
        col4=0;
        key = rowscan();
        if (key!=0){
            sevseg(key+1);
        }
        col1=0;
        col2=0;
        col3=1;
        col4=0;
        key = rowscan();
        if (key!=0){
            sevseg(key+2);
        }

        col1=0;
        col2=0;
        col3=0;
        col4=1;
        key = rowscan();
        if (key!=0){
            sevseg(key+3);
        }

    }
}
