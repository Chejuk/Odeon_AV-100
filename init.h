/* 
 * File:   init.h
 * Author: anatoly
 *
 * Created on 23 february 2018 , 11:12
 */

#ifndef INIT_H
#define	INIT_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <htc.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif

 
#define MAINT_INTERRUPT_TIMEOUT_MS  4
#define TEST(x) PORTBbits.RB3 = x
    
    extern volatile uint8_t portA;
    extern volatile uint16_t dVal;



#ifdef	__cplusplus
}
#endif

#endif	/* INIT_H */

