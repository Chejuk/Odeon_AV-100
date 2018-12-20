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
#include "eeprom.h"

const static uint8_t selector_addresses = 0x94;
const uint8_t input_channels[5] = {INPUT_6CH, INPUT_AUX, INPUT_STEREO_3, INPUT_STEREO_4, INPUT_FM};

struct Task_Input {
    enum Select_Input type[5];
    uint8_t param[5];
    uint8_t num_set;
    uint8_t num_run;
};

static struct Task_Input    input_task = {{0,0,0,0,0}, {0,0,0,0,0}, 0, 0};

void select_set_task(enum Select_Input task, uint8_t param)
{
    input_task.type[input_task.num_set] = task;
    input_task.param[input_task.num_set] = param;
    input_task.num_set++;
    if(input_task.num_set > 4) input_task.num_set = 0;
}

void select_run()
{
    bool sus;// = false;
    uint8_t dat;
    
    for(uint8_t kx = 0; kx < 5; kx++) {
    
        switch (input_task.type[input_task.num_run]) {
        case Input_Set: {
            dat = 0xC0 | input_channels[parameters.Input];
            sus = true;
        }; break;
    
        case Input_Mute: {
            dat = 0xF0 | (input_channels[parameters.Input] << 1) | input_task.param[input_task.num_run];
            sus = true;
        }; break;
    
        case Input_Surround: {
            dat = 0xD0 | input_task.param[input_task.num_run];
            sus = true;
        }; break;
    
        case Input_Mixed: {
            dat = 0x90 | input_task.param[input_task.num_run];
            sus = true;
        }; break;
    
        case Input_None:
        default :
            sus = false;
        };
    
        if(sus && I2C_Send(selector_addresses, &dat, 1)) {
            input_task.type[input_task.num_run] = Input_None;
            input_task.param[input_task.num_run] = 0;
            input_task.num_run++;
            if(input_task.num_run > 4) input_task.num_run = 0;
        };
    };
 
}

bool input_key_push()
{
#ifdef _DEBUG_
    return false;
#else
    static uint16_t timer_counter = 0;
    
    if((RB3 == 0) && (timer_counter < (uint16_t) INPUT_KEY_TIMEOUTS * 2)) {
        timer_counter++;
    } else if(timer_counter){
        timer_counter--;
    };
    
    if(timer_counter > (uint16_t) INPUT_KEY_TIMEOUTS) {
        timer_counter = 0;
        return true;
    }
    else return false;
#endif
}