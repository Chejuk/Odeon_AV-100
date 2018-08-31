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

//__CONFIG(HS & WDTDIS & LVPDIS & DEBUGEN & WRTEN);    
__CONFIG(FOSC_HS & LVP_OFF & DEBUG_ON & WDTE_OFF);


volatile uint16_t dVal = 0;//xFFFF;
struct Save_Parameters  parameters;

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
    
    //test pin PB3
    TRISB3 = 0;// PDB3 as output
    RB3 = 1;
    
    T0IE = 0;   //Timer0 Interrupt disable
    PEIE = 1; // Peripheral Interrupt Enable bit in INTCON
            
    ir_init();
    leds_init();   
    encoder_init();
    
    __delay_ms(100);
    i2c_init();
    parameters_read();
    //eeprom_init();

    GIE = 1; //interrupt enable

//    symbols_set4((uint16_t) ir_puls.value); 
//    while(1) {};
    
//  uint8_t time = 0;
 //   volatile uint16_t disp = 0;
 
 //   dVal =  9012;
    
   	for(;;) {
        main_run();
//		CLRWDT();	// Idly kick the dog
//        time++;
        __delay_ms(50);
//        symbols_set4(time);
//        symbol_setValue(ir_puls.count);
//        symbol_setValue(time);
//        if(ir_puls.new) {
////            symbols_set4((uint16_t) ir_puls.value);
//        if(disp != dVal) {
//            disp = dVal;
//            symbols_set4(disp);
//        };
//            ir_puls.new = false;
//        };
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
    //    PORTBbits.RB3 = !PORTBbits.RB3;
//        dVal += encoder_inc();
//        //leds_next();        
//        if(!ir_check()) 
//            leds_next();
        
        timer_interrupt_run();
        TMR2IF = 0;
   };    
        
  //  GIE = 1; // Global interrupts enable
}