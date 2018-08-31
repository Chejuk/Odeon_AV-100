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

static const uint8_t volume_ctrl_addresses = 0x88;

bool value_common(uint8_t Value)
{
    if(Value > 80)
        Value = 80;
    //parameters.Value = Value;
    //parameters.crc = Crc8(&parameters, sizeof(parameters) - 1);
    
    uint8_t data[7];
    data[0] = 0x10;
    
    for(uint8_t kx = 0; kx < 6; kx++) {
        int8_t val = 80 - (Value + parameters.OutValues[kx]);
        if(val < 0) val = 0;
        if(val > 80) val = 80;
        if(val == 80) {
            data[kx + 1] = 0xC0;
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
    
    parameters.OutValues[num] = value;
//    parameters.crc = Crc8((uint8_t*) &parameters, sizeof(parameters) - 1);
    
    uint8_t data[2];
    data[0] = num;
    data[1] = 80 - (parameters.Value + value);
    if(data[1] > 80) data[1] = 0;
    else if(data[1] > 71) 
        data[1] = 0x80 + (data[1] - 72);
    
return I2C_Send(volume_ctrl_addresses, data, 2);
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