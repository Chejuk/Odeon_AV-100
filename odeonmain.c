/* 
 * File:   odeonmain.c
 * Author: anatoly
 *
 * Created on 22 february 2018 12:34
 */

#include <htc.h>
#include <stdint.h>

#include "init.h"
#include "tools/ledsdisp.h"
#include "tools/ir.h"
#include "tools/tools.h"
#include "tools/i2c.h"
#include "tools/eeprom.h"
#include "tools/volume_ctrl.h"

//__CONFIG(HS & WDTDIS & LVPDIS & DEBUGEN & WRTEN);    
__CONFIG(FOSC_HS & LVP_OFF & DEBUG_ON & WDTE_OFF);


volatile uint16_t dVal = 0;//xFFFF;
struct Save_Parameters  parameters;

#ifdef _DEBUG_
uint8_t debug_value = 0;
#endif

/*
 * Main priject function
 */
int main(int argc, char** argv) {
      
    GIE = 0; //interrupt disable

    // Timer2 setting to MAINT_INTERRUPT_TIMEOUT_MS = 5 msec(200 Hz) from 5MHz
    TMR2ON = 0;     //timer2 off
    // Set common devideto upto 25000
    PR2 = -179;     // PR2 devider; (-159) = 5msec; (-179) = 4 msec; 
    T2CON = 0x7B;   // 7b pre(16) and post(8) scale  to max upto 128
    TMR2ON = 1;     // timer2 ON   
    TMR2IE = 1;
    
#ifdef _DEBUG_  
    //test pin PB3
    TRISB3 = 0;// PDB3 as output
#else
    TRISB3 = 1;// PDB3 as input; Audio input select
#endif
    RB3 = 1;

    
    //Mute output pin
    TRISA5 = 0; //Set RA5 as output
    RA5 = 1;    //Mute On
    
    T0IE = 0;   //Timer0 Interrupt disable
    PEIE = 1; // Peripheral Interrupt Enable bit in INTCON
            
    ir_init();
    leds_init();   
    encoder_init();
    
    i2c_init();
    parameters_read();
    __delay_ms(400);

    GIE = 1; //interrupt enable
    
    //Main loop
   	for(;;) {
        
        switch (state) {
        case State_Off : {
            leds_clr();
            led_switch(LED_POWER, true);
            led_switch(LED_7 | LED_8, false);
            //Mute_Off = 1;
            portA |= (1 << 5);
        }; break;
        
        case State_Volume: {
            led_switch(LED_POWER, false);
            led_switch(LED_7 | LED_8, true);
#ifdef _DEBUG_
            symbols_set4((uint8_t) debug_value);
#else
            symbol_setValue(parameters.Value);
#endif
            channel_out_view();
            volume_set_run();
        }; break;
        
        case State_Channel_Vol: {
            led_switch(LED_POWER, false);
            led_switch(LED_7 | LED_8, true);
        
            if(channel_num == 0) {
                symbol_setValue(parameters.OutValues[VOLUME_SUB - 1]);
            } else if(channel_num == 1) {
                symbol_setValue(parameters.OutValues[VOLUME_SL - 1]);
            } else if(channel_num == 2) {
                symbol_setValue(parameters.OutValues[VOLUME_CT - 1]);
            } else if(channel_num == 3) {
                symbol_setValue(parameters.OutValues[VOLUME_FL - 1]);
            };
        
            channel_value_view();
//            volume_set_run();
        }; break;
     
        default:
            state = State_Off;
        };
    
        select_run();
    
 //       if(!mute_on) led_switch(LED_MUTE , false);//		CLRWDT();	// Idly kick the dog
        __delay_ms(50);
    };

}


static void interrupt isr(void)	// Here be interrupt function - the name is unimportant.
{
 //   GIE = 0; // global interrupts disable
    
	if(T0IF) {		// Was this a timer overflow?
        T0IF = 0;		// Clear interrupt flag, ready for next
    };
    
    //Comparator interrupt
    // On IR signal
    if(CMIF) {
        ir_isr();
        //ADIF = 0;
        CMIF = 0;
    }   
    
    //Timer interrupt
    // On main timer tick
    if(TMR2IF) {        
        timer_interrupt_run();
        TMR2IF = 0;
   };    
        
  //  GIE = 1; // Global interrupts enable
}