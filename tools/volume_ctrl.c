/* 
 * File:   volume_ctrl.c
 * Author: root
 *
 * Created on 29 august 2018 , 12:37
 */

#include <htc.h>

#include "volume_ctrl.h"
#include "i2c.h"
#include "eeprom.h"
#include "tools.h"

struct Volume_Task  {
    uint8_t channel[5];
    int8_t  value[5];
    uint8_t num_set;
    uint8_t num_run;
} ;

static struct Volume_Task volume_task = {{5,5,5,5,5}, {0,0,0,0,0}, 0, 0};

static const uint8_t volume_ctrl_addresses = 0x88;

bool value_common(uint8_t Value)
{
    if(Value > 80)
        Value = 80;
    
    uint8_t data[7];
    data[0] = 0x10;
    
    for(uint8_t kx = 0; kx < 6; kx++) {
        int8_t val;
        if(Value == 0) val = 80;
        else
            val = 80 - (Value + parameters.OutValues[kx]);
        
        if(val < 0) val = 0;
        if(val > 80) val = 80;
        if(val == 80) {
            data[kx + 1] = 0xC0;
            val = 0;
        } else if(val > 71) {
            data[kx + 1] = 0x80;
            val -= 72;
        } else data[kx + 1] = 0;
        data[kx + 1] += val;
    };
    
return I2C_Send(volume_ctrl_addresses, data, 7);
}

bool value_channel(uint8_t num, int8_t value)
{
    if(num > 6) return false;
    
    if(value > CHANNEL_VOLUME_MAX) value = CHANNEL_VOLUME_MAX;
    if(value < -CHANNEL_VOLUME_MAX) value = -CHANNEL_VOLUME_MAX;
    
    uint8_t data[2];
    data[0] = num;
    data[1] = 80 - (parameters.Value + value);
    if(data[1] > 80) data[1] = 0;
    else if(data[1] > 71) 
        data[1] = 0x80 + (data[1] - 72);
    
return I2C_Send(volume_ctrl_addresses, data, 2);
}

void volume_set_task(uint8_t channel, int8_t value)
{
    volume_task.channel[volume_task.num_set] = channel;
    volume_task.value[volume_task.num_set] = value;
    volume_task.num_set++;
    if(volume_task.num_set > 4) volume_task.num_set = 0;
}

void volume_set_run()
{
    bool sus;
    uint8_t num;
    
    for(uint8_t kx = 0; kx < 5; kx++) {
        sus = false;
        num = volume_task.num_run;
        
        if(volume_task.channel[num] == 0) {
            sus = value_channel(VOLUME_SUB - 1, volume_task.value[num]);    
        
        } else if(volume_task.channel[num] == 1) {
            sus = value_channel(VOLUME_FL - 1, volume_task.value[num]) &&
                  value_channel(VOLUME_FR - 1, volume_task.value[num]) &&
                  value_channel(VOLUME_SL - 1, volume_task.value[num]) &&
                  value_channel(VOLUME_SR - 1, volume_task.value[num]);
        
        } else if(volume_task.channel[num] == 2) {
            sus = value_channel(VOLUME_CT - 1, volume_task.value[num]);
        
        } else if(volume_task.channel[num] == 3) {
            sus = value_channel(VOLUME_FL - 1, volume_task.value[num]) &&
                  value_channel(VOLUME_FR - 1, volume_task.value[num]);
        
        } else if(volume_task.channel[num] == 4) {
            sus = value_common(volume_task.value[num]);
            //sus = value_channel(VOLUME_FL - 1, volume_task.value);
        };
    
        if(sus) {
            volume_task.channel[num] = 5;
            volume_task.value[num] = 0;
            volume_task.num_run++;
            if(volume_task.num_run > 4) volume_task.num_run = 0;
        };
        
    };    
}


void parameters_outval_add(uint8_t num, int8_t val)
{
    if(num > 5) return;
    
    parameters.OutValues[num] += val;
    if(parameters.OutValues[num] > (int8_t) CHANNEL_VOLUME_MAX)
        parameters.OutValues[num] = CHANNEL_VOLUME_MAX;
    else if(-parameters.OutValues[num] > (int8_t) CHANNEL_VOLUME_MAX)
        parameters.OutValues[num] = -CHANNEL_VOLUME_MAX;
}