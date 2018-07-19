/* 
 * File:   ledsdisp.c
 * Author: anatoly
 *
 * Created on 28 february 2018 12:08
 */

#include <htc.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "../init.h"
#include "ledsdisp.h"

/*   A
 * F   B
 *   G
 * E   C
 *   D   P 
 *  */

#define LED_A   (1 << 2)
#define LED_B   (1 << 4)
#define LED_C   (1 << 5)
#define LED_D   (1 << 6)
#define LED_E   (1 << 7)
#define LED_F   (1 << 3)
#define LED_G   (1 << 0)
#define LED_P   (1 << 1)

//disply symbols defines
const uint8_t symbols[] = {(LED_A | LED_B | LED_C | LED_D | LED_E | LED_F),
(LED_B | LED_C ), 
(LED_A | LED_B | LED_G | LED_E | LED_D ),// 2 
(LED_A | LED_B | LED_G | LED_C | LED_D ),// 3
(LED_F | LED_B | LED_G | LED_C ),        // 4
(LED_A | LED_G | LED_C | LED_D | LED_F), // 5 
(LED_A | LED_G | LED_C | LED_D | LED_E | LED_F), // 6
(LED_A | LED_B | LED_C ), // 7
(LED_A | LED_B | LED_C | LED_D | LED_E | LED_F | LED_G),//8
(LED_A | LED_B | LED_C | LED_D | LED_G | LED_F), // 9
(LED_A | LED_G | LED_E | LED_F), // F
(LED_A | LED_C | LED_D | LED_G | LED_F), // S
(LED_A | LED_E | LED_D | LED_F), // C
(LED_C | LED_E | LED_D | LED_F | LED_B), // U
(LED_A | LED_F | LED_G | LED_E | LED_D), // E
(LED_A | LED_F | LED_G | LED_B | LED_E | LED_C), // R
}; 

uint8_t disp[5] = {0,0,0,0,0};
volatile uint8_t portA = 0;


void leds_init()
{
    PORTC = 0x00;
    TRISC = 0x00; //as output
    
    TRISA &= ~(0x0E); //RA 1-3 as output
    TRISB &= ~(0x03);   // RB 0,1 as output
    
    memset(disp, symbols[8] + LED_P, sizeof(disp));
    
    SymbolOff();
    /*
    SymbolSetNum(0,8);
    SymbolSetNum(1,1);
    SymbolSetNum(2,2);
    SymbolSetNum(3,4);
    SymbolAdd(4,(LED_POWER | LED_7 | LED_8));
    */
    leds_next();
 }

void leds_next()
{
    static uint8_t c_rank = 0;
    
    SymbolOff();
 
    switch (c_rank) {
    case 0: 
        portA &= ~0x04;
        break;
    case 1:
        portA &= ~0x08;
        break;
    case 2:
        portA &= ~0x02;
        break;
    case 3:
        RB0 = 0;
        break;
    case 4:
        RB1 = 0;
        break;
    default: c_rank = 0;
    };
    
    if(c_rank < 5)
        PORTC = disp[c_rank];
    PORTA = portA;
        
    c_rank++;
    if(c_rank > 4) c_rank = 0;
}

void symbol_setValue(uint8_t value)
{
    uint8_t tv;
    tv = (uint8_t) (value % 10);
    SymbolSetNum(3,tv);
    tv = (uint8_t) ((value/10) % 10);
    SymbolSetNum(2,tv);
}

void symbols_set4(uint16_t val)
{
    uint8_t tv;
    uint16_t tv16 = val;
    
    tv = (uint8_t) (tv16 % 10);
    SymbolSetNum(3,tv); 
    tv16 /= 10;  tv = (uint8_t) (tv16 % 10);
    SymbolSetNum(2,tv);
    
    tv16 /= 10;  tv = (uint8_t) (tv16 % 10);
    SymbolSetNum(1,tv);
    tv16 /= 10;  tv = (uint8_t) (tv16 % 10);
    SymbolSetNum(0,tv);
    
}