/* 
 * File:   encoder.c
 * Author: Anatoly Palto
 *
 * Created on 17/07/2018, 10:52
 */

#include <htc.h>
#include <stdint.h>
#include "tools.h"
#include "../init.h"

void encoder_init()
{
    TRISB |= 0x30;    // PORTB RB4, RB5 as input 
    PORTB |= 0x30;    // set RB4, RB5 to hight
}

signed char encoder_inc()
{
    static int8_t   result = 0;
    static uint8_t  prevState = 0;
  
    uint8_t stateSet = (PORTB & 0x30) >> 4;

    switch(stateSet) {
	case 2: {
		if(prevState == 3) result++;
		if(prevState == 0) result--; 
		}; break;
 
	case 0: {
		if(prevState == 2) result++;
		if(prevState == 1) result--; 
		}; break;
    
	case 1: {
		if(prevState == 0) result++;
		if(prevState == 3) result--; 
		}; break;
        
	case 3:	{
		if(prevState == 1) result++;
		if(prevState == 2) result--; 
		}; break;
	};
    
   prevState = stateSet;
    
    if(result >= 4) {
        result = 0;
        return 1;
    } else if(result <= -4) {
        result = 0;
        return -1;        
    } else
        return 0;

};