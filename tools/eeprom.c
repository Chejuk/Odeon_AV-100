/* 
 * File:   eeprom.c
 * Author: Anatoly Palto
 * 24C02 chip; 256 bytes
 * Created on 19 07 2018 , 12:18
 */

#include <htc.h>
#include <string.h>
#include "eeprom.h"

#include "../init.h"
#include "i2c.h"
#include "tools.h"
#include "inputset.h"

//static const uint8_t    eeprom_address = 0xA0;//0x94;//0xA0;
//static bool             eeprom_enable = false;
//static bool             eeprom_valid = false;
//static uint8_t eeprom_data[16];

#define FLAG    0x0D
#define START_ADDRESSES 0x02

void parameters_write()
{
    parameters.flag = FLAG;
    parameters.crc = Crc8((uint8_t*) &parameters, sizeof(parameters) - 1);
    
    uint8_t addr = START_ADDRESSES;
    uint8_t*    pp = (uint8_t*) &parameters;
    for(uint8_t kx = 0; kx < sizeof(parameters); kx++) {
        eeprom_write(addr, *pp);
        pp++;
        addr++;
    }
}

void parameters_read()
{
    uint8_t kx;
    uint8_t addr = START_ADDRESSES;
    uint8_t*    pp = (uint8_t*) &parameters;
    for(kx = 0; kx < sizeof(parameters); kx++) {
        *pp = eeprom_read(addr);
        pp++;
        addr++;
    };
    
    uint8_t crc = Crc8((uint8_t*) &parameters, sizeof(parameters) - 1);
    if((crc != parameters.crc) && (parameters.flag != FLAG)) {
        // init default parameters;
        parameters.Input = 0;
        parameters.Value = 15;
        for(kx = 0; kx < 6; kx++) {
            parameters.OutValues[kx] = 0;
        };
        parameters_write();
    }
}

//bool eeprom_byte_write(uint8_t byte_addr, uint8_t byte);


/*
void eeprom_init()
{
 //   i2c_reset();
    
    while(!eeprom_valid) {
        eeprom_enable = I2C_Read(eeprom_address, &eeprom_data, 1);
//    
        if(eeprom_data[0] == FLAG) eeprom_valid = true;
        else eeprom_valid = false;
//        uint8_t dat = 0xff;
 //       i2c_block_write(eeprom_address, &dat, 1, true);
        
       __delay_ms(1000);
    };
    
    if(!eeprom_valid) eeprom_byte_write(0, FLAG);
    
}

bool eeprom_data_read(uint8_t dat_addr, uint8_t* data, uint8_t len)
{
    return I2C_Read(eeprom_address, data, len); // TODO: wrong operation
          
}

bool eeprom_data_write(uint8_t dat_addr, uint8_t* data, uint8_t len)
{
    memcpy(&eeprom_data[1], data, len);
    eeprom_data[0] = dat_addr;
    
    return I2C_Send(eeprom_address, eeprom_data, len + 1);
}

bool eeprom_byte_write(uint8_t byte_addr, uint8_t byte)
{
    uint8_t sb = byte;
    return I2C_Send(byte_addr, &sb, 1);
}
*/
uint8_t Crc8(uint8_t *pcBlock, uint32_t len)
{
    uint8_t crc = 0xFF;
    uint32_t i;

    while (len--)  {
        crc ^= *pcBlock++;

        for (i = 0; i < 8; i++)
            crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
    }

    return crc;
}
