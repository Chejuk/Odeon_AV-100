/* 
 * File:   ir.h
 * Author: root
 *
 * Created on 7 ????? 2018 ?., 11:02
 */

#ifndef IR_H
#define	IR_H

#include <htc.h>
#include <stdint.h>
#include <stdbool.h>

#define ADC_HIGH    580
#define ADC_LOW     200

#define TIMEOUT_ACC     60//45
#define TIMEOUT_IDLY    15000

// 20 MHz /4 = 5MHz / prescale 8 = 625 kHz =>  1 msec = 625 tick
#define TIMEOUT_START   7040//7021..7058:7040 +/- 19;       7188 = 11.5 msec

#define TIMEOUT_PREP    3518 //3503 .. 3535: 3518 +/- 15;   2813 = 4.5 msec
#define TIMEOUT_LOW     371  //345 .. 397: 371 +/- 28;      406 = 0.65 msec

#define TIMEOUT_VAL_HIGH    1055 //1028..1082: 1055 +/- 27; 1031 = 1.65 msec;
#define TIMEOUT_VAL_LOW     359  //335..383; 359 +/- 24;    313 = 0.5 msec

#define TIMEOUT_REPEATE_START  5635 // 5527..5575;  9 msec
//#define TIMEOUT_REPEATE_PREP   390   // 2.2 msec
//#define TIMEOUT_REPEATE_LOW         // 0.6 msec


#ifdef	__cplusplus
extern "C" {
#endif
    struct IrPuls {
//        uint8_t state;
        bool    start;
        bool    stop;
        bool    new;
        uint8_t count;
        bool    repeate;
        uint32_t    value;
    };

    extern struct IrPuls ir_puls;
    
    extern void ir_init();
    extern void ir_isr();
    extern void ir_setIdle();
    extern bool ir_check();

#ifdef	__cplusplus
}
#endif

#endif	/* IR_H */

