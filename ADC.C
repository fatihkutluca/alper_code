/*
 * File:   week5.c
 * Author: alperssisman
 *
 *
 */

#include <pic18f452.h>
#include <htc.h>
#include "delay.h"
#define _XTAL_FREQ 8000000
#pragma config OSCS = OFF, OSC=HS, BOR = OFF, PWRT=OFF,WDT=OFF,CCP2MUX=OFF,LVP=OFF,STVR=OFF

#define RS RD0
#define EN RD1

// LCD ROUTINES//

void LCD_STROBE(void) {
    EN = 1;
    DelayUs(1);
    EN = 0;
}

void DATA(unsigned char c) {
    RS = 1;
    DelayUs(50);
    PORTD = PORTD & 0xC3;
    PORTD = PORTD | ((c & 0xF0) >> 2);
    LCD_STROBE();
    PORTD = PORTD & 0xC3;
    PORTD = PORTD | ((c & 0x0F) << 2);
    LCD_STROBE();
}

void CMD(unsigned char c) {
    RS = 0;
    DelayUs(50);
    PORTD = PORTD & 0xC3;
    PORTD = PORTD | ((c & 0xF0) >> 2);
    LCD_STROBE();
    PORTD = PORTD & 0xC3;
    PORTD = PORTD | ((c & 0x0F) << 2);
    LCD_STROBE();
}

void CLR(void) {
    CMD(0x01);
    DelayMs(2);
}

void LCD_INIT(void) {
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

void string1(char *q) {
    while (*q) {
        DATA(*q++);
    }
}

////////////////////////////////////////////////

char int2str(int q) {
    char strr[6] = "*****\0", i;
    int onuzeri[4] = {1, 10, 100, 1000};
    for (i = 0; i < 5; i++) {
        if (i == 3)
            strr[4 - i] = ',';
        else if (i == 4)
            strr[4 - i] = 0x30 + (((int) (q / onuzeri[3])) % 10);
        else
            strr[4 - i] = 0x30 + (((int) (q / onuzeri[i])) % 10);

    }
    return strr;
}

int main() {
    int ADC_value, voltage;
    ADCON1 = 0x8E;
    ADCON0 = 0x00;
    TRISA = 0X01;
    TRISD = 0x00;
    DelayMs(250);
    LCD_INIT();
    DelayMs(200);
    while (1) {

        ADON = 1;
        DelayUs(100);
        GO_DONE = 1;
        while (GO_DONE);
        ADC_value = ADRESL + 256 * ADRESH;
        ADON = 0;
        voltage = (int) ((float) (ADC_value)*(5.0 / 1.024));
        CMD(0x80);
        string1("Gerilim:");
        string1(int2str(voltage));
    }
}
