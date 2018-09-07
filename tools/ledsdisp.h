/* 
 * File:   ledsdisp.h
 * Author: anatoly
 *
 * Created on 28 february 2018 , 12:09
 */

#ifndef LEDSDISP_H
#define	LEDSDISP_H

#include <stdint.h>
#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "../init.h"
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
    
#define LED_51      (1 << 0)
#define LED_AUX     (1 << 1)
#define LED_RADIO   (1 << 2)
#define LED_MODE    (1 << 3)
#define LED_POWER   (1 << 4)
#define LED_MUTE    (1 << 5)
#define LED_7       (1 << 6)
#define LED_8       (1 << 7)

    
#define SymbolSetNum(r,s)  if((uint8_t) (r) < 5) {TMR2IE = 0; disp[r] = (uint8_t) (symbols[s]); TMR2IE = 1;}  
#define SymbolSet(r,s)  if((uint8_t) (r) < 5) {TMR2IE = 0; disp[r] = (uint8_t) (s); TMR2IE = 1; }  

#define SymbolAdd(r,s)  if((uint8_t) (r) < 5) {TMR2IE = 0; disp[r] |= (uint8_t) (s); TMR2IE = 1;}
#define SymbolRm(r,s)   if((uint8_t) (r) < 5) {TMR2IE = 0; disp[r] &= ~((uint8_t) (s)); TMR2IE = 1;}
#define SymbolOff()        TMR2IE = 0; portA |= 0x0E; TMR2IE = 1; PORTB |= 3; PORTC = 0x00
    
    
    extern uint8_t disp[5];
    extern const uint8_t symbols[];
    extern volatile uint8_t portA;
    
    extern void leds_init();
    extern void leds_next();
    extern void symbol_setValue(int8_t value);
    extern void symbols_set4(uint16_t val);
    extern void led_switch(uint8_t leds, bool On);
    extern void leds_clr();

#ifdef	__cplusplus
}
#endif

#endif	/* LEDSDISP_H */

