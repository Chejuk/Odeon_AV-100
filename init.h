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
#include <stdint.h>
#include <stdbool.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif
 
#define MAINT_INTERRUPT_TIMEOUT_MS  4
#define INPUT_KEY_TIMEOUTS          (250 / MAINT_INTERRUPT_TIMEOUT_MS)

#ifdef _DEBUG_    
    #define TEST(x)     RB3 = x
#else
    #define TEST(x)
#endif
#define Mute_Off    RA5
    
    enum State {
        State_Off,
        State_On,
        State_Volume,
        State_Channel_Vol,
    };
    
#ifdef _DEBUG_
extern uint8_t debug_value;
#endif
    
    extern volatile uint8_t portA;
    extern enum State state; 
 //   extern bool mute_on;
    extern uint8_t  channel_num;

    extern void channel_out_view();
    extern void channel_value_view();

//    extern void main_run();
    extern void timer_interrupt_run();
    


#ifdef	__cplusplus
}
#endif

#endif	/* INIT_H */

