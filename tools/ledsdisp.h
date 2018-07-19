/* 
 * File:   ledsdisp.h
 * Author: anatoly
 *
 * Created on 28 february 2018 , 12:09
 */

#ifndef LEDSDISP_H
#define	LEDSDISP_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "../init.h"
    
#define LED_51      (1 << 0)
#define LED_AUX     (1 << 1)
#define LED_RADIO   (1 << 2)
#define LED_21      (1 << 3)
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
    
    extern void leds_init();
    extern void leds_next();
    extern void symbol_setValue(uint8_t value);
    extern void symbols_set4(uint16_t val);

#ifdef	__cplusplus
}
#endif

#endif	/* LEDSDISP_H */

