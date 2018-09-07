/* 
 * File:   ir.c
 * Author: Anatoly Palto
 *
 * Created on 7 march 2018 , 11:00
 */

#include <htc.h>
#include <stdint.h>
#include <stdbool.h>

#include "ir.h"
#include "ledsdisp.h"

#define ADC_HIGH    580
#define ADC_LOW     200

#define TIMEOUT_ACC     40
#define TIMEOUT_IDLY    50000

#define TIMEOUT_START   5130 //6116..6164; 7131 7176
#define TIMEOUT_PREP    1380    // 3440  3476
#define TIMEOUT_LOW     430     // 406 - 407
#define TIMEOUT_HIGH    950

#define TIMEOUT_VAL_LOW     330     // 301 - 304

#define TIMEOUT_REP_PREP    1450
#define TIMEOUT_REP_LOW     390


static uint32_t counter_timer1 = 0;

static enum {
    Idle,
    Start,
    Prep,
    Puls_low,
    Puls_high,
} state;

static struct IrPuls {
    uint8_t state;
    bool    start;
    bool    stop;
    bool    new;
    uint32_t    value;
    uint8_t count;
    bool    repeate;
} ir_puls;

void init_ir()
{   
    // ADC init, AN0
    TRISA |= 0x01;    // ????????? RA0  ??? ????
    PORTA &= ~0x01;     // set RA0 to low
    //Right justified. 
    //FOSC /16; Vref=Vdd; AN0
    ADCON1 = 0xCE;
    ADCON0 = 0x41; //On chanel0
    ADIF = 0;
    ADIE = 1;
    PEIE = 1;
    
     //Timer1 init
    // 1:8 prescale value; Internal clock; Oscillator is enabled
    TMR1IE = 0;//the timer1 interrupt disable
    T1CON = 0x30; //
    TMR1 = 0x0000;
    TMR1ON = 1; // Start TMR1
    
    GO_DONE = 1; //Start ADC conversion
}

void isr_ir()
{
    uint16_t adc_val = (ADRESH & 0x03) * 256 + ADRESL;
    static uint16_t prevTimer = 0;
    uint16_t timeout = TMR1 - prevTimer;
    static bool    puls_low = false;
    bool    was_level_change = false;
        
    if(timeout > (uint16_t) TIMEOUT_IDLY) {
       if(state == Puls_low) {
            ir_puls.stop = true;
            ir_puls.new = true;
//           symbols_set4((uint16_t) timeout);
        };
        state = Idle;
     //   prevTimer = TMR1;
    };
    
    if(adc_val > (uint16_t) ADC_HIGH) {
        was_level_change = puls_low; 
        puls_low = false; 
    } else {
        was_level_change = !puls_low;
        puls_low = true;
//        SymbolSetNum(0, 10);
    };
    
    static uint16_t t_t = 0;
    
    if(was_level_change) {
        prevTimer = TMR1;
    
        switch (state) {
        case Start:
            if(!puls_low
                && (timeout > (uint16_t) (5500 - 500))// (TIMEOUT_START - TIMEOUT_ACC)) 
                && (timeout < (uint16_t) (5500 + 500))//(TIMEOUT_START + TIMEOUT_ACC))
                ) {
                state = Prep;
   //             symbols_set4((uint16_t) timeout);
            } 
            break;
            
        case Prep:
            if(puls_low
                    && (timeout > (uint16_t) (1380 - 100)) // (TIMEOUT_PREP - TIMEOUT_ACC)) 
                    && (timeout < (uint16_t) (1380 + 100)) //(TIMEOUT_PREP + TIMEOUT_ACC))
                ){
 //               symbols_set4((uint16_t) timeout);
                state = Puls_low;
            }
            break;
            
        case Puls_low:
            if(!puls_low
                    && (timeout > (uint16_t) (415 - 30))// (TIMEOUT_LOW - TIMEOUT_ACC)) 
                    && (timeout < (uint16_t) (415 + 30)) //(TIMEOUT_LOW + TIMEOUT_ACC))
                ){
//                symbols_set4((uint16_t) timeout);
                state = Puls_high;
                t_t = 0xFFFF;
            }
            break;
            
        case Puls_high:
            if(puls_low) {
                if((uint16_t) timeout < t_t) t_t = (uint16_t) timeout;
                symbols_set4((uint16_t) t_t);
                if((timeout > (uint16_t) (TIMEOUT_HIGH - TIMEOUT_ACC)) 
                  //  && (timeout < (uint16_t) (TIMEOUT_HIGH + TIMEOUT_ACC))
                        ) {
                    state = Puls_low;
                    ir_puls.count++;
                    ir_puls.value <<= 1;
                    ir_puls.value += 1;
                } else if((timeout > (uint16_t) (TIMEOUT_LOW - TIMEOUT_ACC)) 
                    && (timeout < (uint16_t) (TIMEOUT_LOW + TIMEOUT_ACC))
                        ) {
                    ir_puls.value <<= 1;
                    state = Puls_low;
                    ir_puls.count++;
                } else 
                    state = Idle;                
            };           
            break;
            
        case Idle:
        default:
            if(puls_low) {
                state = Start;
                ir_puls.new = false;
                ir_puls.start = true;
                ir_puls.stop = false;
                ir_puls.repeate = false;
                ir_puls.value = 0;
                ir_puls.count = 0;
            } else {
                SymbolSetNum(0, 11);
            }
            
        };
        
        was_level_change = false;
    };

    GO_DONE = 1;// start next conversion
}