/* 
 * File:   tools.h
 * Author: root
 *
 * Created on 17 ???? 2018 ?., 11:02
 */

#ifndef TOOLS_H
#define	TOOLS_H

#include <stdint.h>
#include <stdbool.h>

#include "inputset.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
    struct Task_Input {
        enum Select_Input type;
        uint8_t param;
    };

    extern struct Save_Parameters  parameters;

    extern void encoder_init();
    extern signed char encoder_inc();


#ifdef	__cplusplus
}
#endif

#endif	/* TOOLS_H */

