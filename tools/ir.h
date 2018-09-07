/* 
 * File:   ir.h
 * Author: Anatoly Palto
 *
 * Created on 7 march 2018, 11:02
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

#define CODE_ST_BY           (uint8_t) 128
#define CODE_MUTE            (uint8_t) 130
#define CODE_INPUT_SEL       (uint8_t) 132
#define CODE_RESET           (uint8_t) 134
#define CODE_MODE            (uint8_t) 136
#define CODE_AM_FM           (uint8_t) 138
#define CODE_TUNE_PR         (uint8_t) 140
#define CODE_TUNE_NX         (uint8_t) 142
#define CODE_MEMORY          (uint8_t) 129
#define CODE_PRESELECT_PR    (uint8_t) 131
#define CODE_PRESELECT_NX    (uint8_t) 133
#define CODE_VOLUME_UP       (uint8_t) 135
#define CODE_CHANNEL_UP      (uint8_t) 137
#define CODE_VOLUME_DOWN     (uint8_t) 139
#define CODE_CHANNEL_DOWN    (uint8_t) 141


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
        uint8_t code;
    };

    extern struct IrPuls ir_puls;
    
    extern void ir_init();
    extern void ir_isr();
    extern void ir_setIdle();
    extern bool ir_check();
    extern uint8_t ir_code_get();

#ifdef	__cplusplus
}
#endif

#endif	/* IR_H */

