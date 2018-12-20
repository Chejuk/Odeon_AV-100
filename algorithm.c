/* 
 * File:   algorithm.c
 * Author: anatoly
 *
 * Created on 30 august 2018 , 11:06
 */

#include <htc.h>
#include <stdint.h>
#include <string.h>

#include "init.h"
#include "tools/tools.h"
#include "tools/ir.h"
#include "tools/ledsdisp.h"
#include "tools/eeprom.h"
#include "tools/volume_ctrl.h"


enum State state; 
static bool mute_on = false;
static bool mode_on = false;
uint8_t  channel_num = 0;

void channel_out_view();
void channel_value_view();

// Timer interrupt algorithm run function
void timer_interrupt_run()
{
    static int8_t enVal = 0;
    static uint16_t change_counter = 0xF000;
    static uint16_t mute_timer = 0;
    static uint16_t channel_timeout = 0;
    
    uint8_t ir_code = 0;
    
    enVal += encoder_inc();
    
    if(ir_check()) return;
        leds_next();
    
    ir_code = ir_code_get();
#ifdef _DEBUG_
    if(ir_code)
        debug_value = ir_code;
#endif
    
    if(mute_on) {
        mute_timer++;
        if(mute_timer < (200 / MAINT_INTERRUPT_TIMEOUT_MS))
            led_switch(LED_MUTE , true);
        else if(mute_timer < (400 / MAINT_INTERRUPT_TIMEOUT_MS))
            led_switch(LED_MUTE , false);
        else mute_timer = 0; 
        //Mute_Off = 1;
        portA |= (1 << 5);
    } else {
        //Mute_Off = 0;
        portA &= ~(1 << 5);
    };
   
    //State select
    switch (state) {
    case State_Off: {
        if(ir_code == CODE_ST_BY) 
            enVal = 1;
        if(enVal != 0) {
            state = State_Volume;
            enVal = 0;
            select_set_task(Input_Set, parameters.Input);
            mute_on = false;
            select_set_task(Input_Mute, 0);
            volume_set_task(4, parameters.Value);
            select_set_task(Input_Surround, 0);
        }
    }; break; 
    
    case State_Volume : {
        if(ir_code == CODE_ST_BY) {
            state = State_Off;
            volume_set_task(4, 0);
            select_set_task(Input_Mute, 1);
            mute_on = false;
            
        } else if(ir_code == CODE_VOLUME_UP)
            enVal++;
        else if(ir_code == CODE_VOLUME_DOWN)
            enVal--;
        else if((ir_code == CODE_INPUT_SEL) 
                || input_key_push())  {
            parameters.Input += 1;
            if(parameters.Input > 3) parameters.Input = 0;
            select_set_task(Input_Set, parameters.Input);
        } else if(ir_code == CODE_MUTE) {
            mute_on = !mute_on;
            select_set_task(Input_Mute, (mute_on)?1:0);
            if(!mute_on) led_switch(LED_MUTE , false);
        } else if(ir_code == CODE_CHANNEL_UP) {
            channel_num++;
            if(channel_num > 5) channel_num = 0;
            state = State_Channel_Vol;
            channel_timeout = 0;
        } else if(ir_code == CODE_CHANNEL_DOWN) {
            if(channel_num > 0) channel_num--;
            else channel_num = 5;
            state = State_Channel_Vol;
            channel_timeout = 0;
        } else if(ir_code == CODE_MODE) {
            mode_on = !mode_on;
            if(mode_on) 
                led_switch(LED_MODE , true);
            else led_switch(LED_MODE , false);
            select_set_task(Input_Surround, (mode_on)?1:0);
        };
                    
        if(enVal != 0) {
            uint8_t pv = parameters.Value;
            parameters.Value += enVal;             
            if(parameters.Value < 0)
                parameters.Value = 0;
            else if(parameters.Value > 80) 
                parameters.Value = 80;
            
            enVal = 0;
            if(pv != parameters.Value) {
                change_counter = 0;
            };
            volume_set_task(4, parameters.Value);
        }; 
               
    }; break;
  
    case State_Channel_Vol: {
        if(ir_code == CODE_CHANNEL_UP) {
            channel_num++;
            if(channel_num > 3) channel_num = 0;
            state = State_Channel_Vol;
            channel_timeout = 0;
        } else if(ir_code == CODE_CHANNEL_DOWN) {
            if(channel_num > 0) channel_num--;
            else channel_num = 3;
            state = State_Channel_Vol;
            channel_timeout = 0;
        } else if(ir_code == CODE_VOLUME_UP)
            enVal++;
        else if(ir_code == CODE_VOLUME_DOWN)
            enVal--;
        else if(ir_code == CODE_RESET) {
            channel_timeout = 0;
            memset(&parameters.OutValues, 0, 6);
            volume_set_task(4, parameters.Value);
        } else if(ir_code == CODE_MUTE) {
            mute_on = !mute_on;
        } else if(ir_code == CODE_INPUT_SEL) {
            state = State_Volume;
        };
        
        if(enVal != 0) {
            channel_timeout = 0;
            if(channel_num == 0) {
                parameters_outval_add(VOLUME_SUB - 1, enVal);
            } else if(channel_num == 1) {
                parameters_outval_add(VOLUME_FL - 1, enVal);
                parameters_outval_add(VOLUME_FR - 1, enVal);
                parameters_outval_add(VOLUME_SL - 1, enVal);
                parameters_outval_add(VOLUME_SR - 1, enVal);
            } else if(channel_num == 2) {
                parameters_outval_add(VOLUME_CT - 1, enVal);
            } else if(channel_num == 3) {
                parameters_outval_add(VOLUME_FL - 1, enVal);
                parameters_outval_add(VOLUME_FR - 1, enVal);                
            };
            volume_set_task(4, parameters.Value);
            enVal = 0;
        };
        
        channel_timeout++;
        if(channel_timeout > (uint16_t) (3000 / MAINT_INTERRUPT_TIMEOUT_MS))
            state = State_Volume;
    };
    }
    
    if(change_counter < (uint16_t) (3000 / MAINT_INTERRUPT_TIMEOUT_MS)) {
        change_counter++;
    } else if(change_counter < 0xF000) {
        parameters_write();
        change_counter = 0xF000;
    };
    
}

void channel_out_view()
{
    switch(parameters.Input) {

    case 0: {// 5.1 input
        SymbolSetNum(0,5);
        SymbolSetNum(1,1);
        SymbolAdd(0, LED_P);
        led_switch(LED_51, true);
        led_switch(LED_AUX, false);
    }; break;
    
    // AUX inputs
    case 1: 
    case 2:
    case 3: {
        led_switch(LED_51, false);
        led_switch(LED_AUX, true);
        SymbolSet(1,0);
        SymbolSetNum(0,parameters.Input);
    }; break;
    
    default:
        parameters.Input = 0;
        led_switch(LED_51, false);
    }
}

void channel_value_view()
{
    switch (channel_num) {
    case 0 : { //SUB
        SymbolSetNum(0,12); //S
        SymbolSetNum(1,14);  //U
    }; break;
    
    case 1 : { //SR surround
        SymbolSetNum(0,12); //S
        SymbolSetNum(1,16);  //R
    }; break;
    
    case 2 : { //CE central
        SymbolSetNum(0,13); //C
        SymbolSetNum(1,15);  //E
    }; break;

    case 3 : { //FR front
        SymbolSetNum(0,11); //F
        SymbolSetNum(1,16);  //R
    }; break;
    
    };
}