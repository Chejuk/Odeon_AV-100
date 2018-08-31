/* 
 * File:   inputset.h
 * Author: root
 *
 * Created on 29 august 2018 , 10:31
 */

#ifndef INPUTSET_H
#define	INPUTSET_H

#include <stdint.h>
#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define INPUT_6CH       (uint8_t) 0x07
#define INPUT_STEREO_1  (uint8_t) 0x0B
#define INPUT_STEREO_2  (uint8_t) 0x0A
#define INPUT_STEREO_3  (uint8_t) 0x09
#define INPUT_STEREO_4  (uint8_t) 0x08
#define INPUT_AUX   INPUT_STEREO_1
#define INPUT_FM    INPUT_STEREO_2
    
const uint8_t input_channels[5] = {INPUT_6CH, INPUT_AUX, INPUT_STEREO_3, INPUT_STEREO_4, INPUT_FM};
    
#define MUTE_FL     0
#define MUTE_FR     1
#define MUTE_CT     2
#define MUTE_SUB    3
#define MUTE_SL     4
#define MUTE_SR     5
#define MUTE_ALL    7
    
    
    enum Select_Input {
        Input_None,
        Input_Set,
        Input_Mute_Enable,
        Input_Mute_Disable,
        Input_Surround,
        Input_Mixed
    };

    extern void select_set_task(enum Select_Input task, uint8_t param);
    extern void select_run();
#ifdef	__cplusplus
}
#endif

#endif	/* INPUTSET_H */

