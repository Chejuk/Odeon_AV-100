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
#include "../init.h"


//static uint32_t counter_timer1 = 0;

static enum {
    Idle,
    Start,
    Prep,
    Puls_low,
    Puls_high,
    Repeate_Prep,
    Repeate_Low,        
} IR_State;

struct IrPuls ir_puls;

void ir_init()
{
    TRISA |= 0x01;    // PORTA RA0 as input
    PORTA |= 0x01;     // set RA0 to hight


    //Comparatot init
    CMCON = 0x06;//0x01; 0x06
    CVRCON = 0x88;
    CMIF = 0;
    CMIE = 1;    
    
     //Timer1 init
    // 1:8 prescale value; Internal clock; Oscillator is enabled
    TMR1IE = 0;//the timer1 interrupt disable
    T1CON = 0x30; //
    TMR1 = 0x0000;
    TMR1ON = 1; // Start TMR1
    
    PEIE = 1;

}

void ir_isr()
{
    static volatile uint16_t prevTimer = 0;
    
    volatile uint16_t        tmr = TMR1;
    volatile uint16_t        timeout = tmr - prevTimer;
    
    volatile static uint8_t   cm_state = 0x40;
    
    if(cm_state == (uint8_t) (CMCON & 0x40)) return;
    
    cm_state = (CMCON & 0x40);
       
    volatile bool            isLow;
    
    if(CMCONbits.C1OUT) {
        isLow = true;
        TEST(1);
    } else {
        isLow = false;
        TEST(0);
    }
    
    switch (IR_State) {
    case Start: 
        if(!isLow) {            
            if((timeout > TIMEOUT_START_MIN) 
                && (timeout < TIMEOUT_START_MAX)
                ) {
                IR_State = Prep;
                prevTimer = tmr;
                ir_puls.start = true;
                TEST(1);

            } else if((timeout > TIMEOUT_REPEATE_MIN) 
              && (timeout < TIMEOUT_REPEATE_MAX)
                ){
                IR_State = Repeate_Prep;
                prevTimer = tmr;
                ir_puls.start = true;
                TEST(1);                

            } else {
                TEST(0); 
                IR_State = Idle;
            };
        };break;
            
    case Prep:
        if(isLow) {
            if((timeout > TIMEOUT_PREP_MIN) 
                && (timeout < TIMEOUT_PREP_MAX)
                ){
                IR_State = Puls_low;
                ir_puls.start = true;
                ir_puls.value = 0;
                ir_puls.count = 0;
                prevTimer = tmr;
                TEST(0);
            } else {
                //IR_State = Idle;
                ir_setIdle();
                TEST(1);
            };
        }; break;
            
    case Puls_low:
        if(!isLow) {
            if((timeout > (uint16_t) (TIMEOUT_LOW - TIMEOUT_ACC))  //TIMEOUT_ACC
              && (timeout < (uint16_t) (TIMEOUT_LOW + TIMEOUT_ACC))
              ){                
                prevTimer = tmr;
                TEST(1);

                if(ir_puls.count == 32) {
                    ir_puls.stop = true;
                    ir_puls.new = true;
                    ir_puls.start = false;
                    IR_State = Idle;
                    ir_puls.code = (uint8_t) (ir_puls.value & 0x000000FF);
                } else {
                    ir_puls.count += 1;
                    ir_puls.value <<= 1; 
                    IR_State = Puls_high;
                };       
            } else {
                TEST(0);
            };
        }; break;
            
    case Puls_high: 
        if(isLow){
            if((timeout > (uint16_t) (TIMEOUT_VAL_HIGH + TIMEOUT_ACC)) 
                && ir_puls.start){
                //IR_State = Idle;
                ir_setIdle();
                TEST(1);
                
            } else if((timeout > (uint16_t) (TIMEOUT_VAL_HIGH - TIMEOUT_ACC)) // TIMEOUT_ACC
              && (timeout < (uint16_t) (TIMEOUT_VAL_HIGH + TIMEOUT_ACC))
                ){
                prevTimer = tmr;
                ir_puls.value += 1L;
                IR_State = Puls_low;
                TEST(0);

            } else if((timeout > (uint16_t) (TIMEOUT_VAL_LOW - TIMEOUT_ACC)) 
                && (timeout < (uint16_t) (TIMEOUT_VAL_LOW + TIMEOUT_ACC))
                ) {
                prevTimer = tmr;
                IR_State = Puls_low;
                TEST(0);
            };
        };
        break;
            
    case Repeate_Prep:{
        prevTimer = tmr;
        IR_State = Repeate_Low;
        TEST(0);
        };break;
        
    case Repeate_Low: {
        ir_puls.stop = true;
        ir_puls.new = true;
        ir_puls.start = false;
        prevTimer = tmr;
        TEST(1);
        IR_State = Idle;

        ir_puls.code = (uint8_t) (ir_puls.value & 0x000000FF);
        }; break;
        
    case Idle:
    default:
        if(isLow) {
            prevTimer = tmr;
            IR_State = Start;
            ir_puls.new = false;
            ir_puls.start = false;
            ir_puls.stop = false;
            ir_puls.repeate = false; 
            TEST(0);
        };
    };
           
//    GO_DONE = 1;// start next conversion
}

void ir_setIdle()
{
    ir_puls.new = false;
    ir_puls.start = false;
    ir_puls.stop = false;
    ir_puls.repeate = false;
    IR_State = Idle;   
    TEST(1);
}

// checking ir paket start
bool ir_check()
{
    static uint8_t ir_start_count = 0;
    if(ir_puls.start) //(IR_State != Idle) 
    {
        ir_start_count++;
        if(ir_start_count > (uint8_t) (85 / MAINT_INTERRUPT_TIMEOUT_MS)) {
            ir_setIdle(); 
            ir_start_count = 0;
            ir_puls.value = 0;
        };
        return true;
    } else {
        ir_start_count = 0;
    }

return false;
}

uint8_t ir_code_get()
{
    static uint16_t power_on_timer = 0;
    static uint16_t  idle_counter = 0;

    if(power_on_timer) power_on_timer--;
    
    if(idle_counter > 0) idle_counter--;
    else if(IR_State == Idle) {
        ir_puls.value = 0;
        ir_puls.code = 0;
    };
    
    if(ir_puls.new) {
        static uint8_t ir_code_prev = 0;
        uint8_t code = ir_puls.code;
 
        if((code == ir_code_prev) && (power_on_timer > 0)) {
            code = 0;
        } else 
            power_on_timer = 200 / MAINT_INTERRUPT_TIMEOUT_MS;
        
        /*
        if(code == CODE_ST_BY) {
            if((code == ir_code_prev) && (power_on_timer > 0)) {
                code = 0;
            } else power_on_timer = 300 / MAINT_INTERRUPT_TIMEOUT_MS;
        } else {
            power_on_timer = 0;
        };
         */         
        
        ir_setIdle();
        ir_code_prev = ir_puls.code;  
        idle_counter = (uint16_t) (1000/MAINT_INTERRUPT_TIMEOUT_MS);
        return code;
    };
    
    return 0;
}