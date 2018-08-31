/* 
 * File:   volume_ctrl.h
 * Author: root
 *
 * Created on 29 august 2018 , 12:37
 */

#ifndef VOLUME_CTRL_H
#define	VOLUME_CTRL_H

#include <stdint.h>
#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define VOLUME_FL   1
#define VOLUME_FR   2
#define VOLUME_CT   3
#define VOLUME_SUB  4
#define VOLUME_SL   5
#define VOLUME_SR   6
    
#define CHANNEL_VOLUME_MAX  9


    extern bool value_common(uint8_t Value);
    extern bool value_channel(uint8_t num, int8_t value);
    extern void parameters_outval_add(uint8_t num, int8_t val);

#ifdef	__cplusplus
}
#endif

#endif	/* VOLUME_CTRL_H */

