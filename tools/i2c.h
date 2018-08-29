/* 
 * File:   i2c.h
 * Author: root
 *
 * Created on 19 ???? 2018 ?., 11:14
 */

#ifndef I2C_H
#define	I2C_H

#include <stdint.h>
#include <stdbool.h>


#ifdef	__cplusplus
extern "C" {
#endif

#define ACK     (uint8_t) 0		// ????? ???????
#define NACK    (uint8_t) 1		// ????? ?? ???????

    extern void i2c_init();
    extern void i2c_reset();
 //   extern bool i2c_block_write(uint8_t address, uint8_t *data, uint8_t len, bool stop);
//    extern bool i2c_data_read(uint8_t device_address, uint8_t data_address, uint8_t *data, uint8_t len);
    extern bool I2C_Read(char Address,char *Data, char Num);
    extern bool I2C_Send(char Address,char *Data,char Num);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

