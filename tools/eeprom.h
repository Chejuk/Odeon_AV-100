/* 
 * File:   eeprom.h
 * Author: Anatoly Palto
 *
 * Created on 19 07 2018 , 12:18
 */

#ifndef EEPROM_H
#define	EEPROM_H

#include <stdint.h>
#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif
    #define FLAG    0x0D

    struct Save_Parameters {
        uint8_t flag;
        uint8_t Input;
        int8_t  Value;
        int8_t  OutValues[6];
        uint8_t crc;
    };

    //extern void eeprom_init();
    extern void parameters_write();
    extern void parameters_read();
    extern uint8_t Crc8(uint8_t *pcBlock, uint32_t len);


#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

