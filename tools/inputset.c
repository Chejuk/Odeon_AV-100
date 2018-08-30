/* 
 * File:   inputset.c
 * Author: root
 *
 * Created on 29 august 2018 , 10:30
 */

#include <htc.h>

#include "tools.h"
#include "inputset.h"
#include "i2c.h"

const static uint8_t selector_addresses = 0x94;
static struct Task_Input    input_task = {0,0};

void select_set_task(enum Select_Input task, uint8_t param)
{
    input_task.type = task;
    input_task.param = param;
}

void select_run()
{
    bool sus = false;
    uint8_t dat;
    
    switch (input_task.type) {
    case Input_Set: {
        dat = 0xC0 | input_task.param;
        sus = true;
    }; break;
    
    case Input_Mute_Enable: {
        dat = 0xF0 | (input_task.param << 1) | 1;
        sus = true;
    }; break;
    
    case Input_Mute_Disable: {
        dat = 0xF0 | (input_task.param << 1);
        sus = true;
    }; break;

    case Input_Surround: {
        dat = 0xD0 | input_task.param;
        sus = true;
    }; break;
    
    case Input_Mixed: {
        dat = 0x90 | input_task.param;
        sus = true;
    }; break;
    
    case Input_None:
    default :
        sus = false;
    };
    
    if(sus && I2C_Send(selector_addresses, &dat, 1)) {
        input_task.type = Input_None;
        input_task.param = 0;
    };        
}