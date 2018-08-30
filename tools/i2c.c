/* 
 * File:   i2c.c
 * Author: Anatoly Palto
 *
 * Created on 19 07 2018 , 11:13
 */

#include <htc.h>

#include "i2c.h"
#include "../init.h"

#define SCL_UP()    (PORTBbits.RB7 = 1)
#define SCL_DOWN()  (PORTBbits.RB7 = 0)
#define SDA_SET(x)  (PORTBbits.RB6 = x)
#define SDA_GET     PORTBbits.RB6
#define SDA_OUT()   (TRISB6 = 0) 
#define SDA_IN()    (TRISB6 = 1) 

#define I2C_DELAY(x) __delay_us(x * 10) 
//it is 20k/sec;   PT2323 - max 100k bits / second

void i2c_start(void);
void i2c_stop(void);


void i2c_init()
{
    TRISB7 = 0; //set RB7 as output
    TRISB6 = 0; //set RB6 (SDA) as output
    OPTION_REGbits.nRBPU = 1; //PORTB pull up
   /* 
    char data[2];
    //data[0] = 0xFF;// Mute on for address 0x94 pt2323
    __delay_ms(500);
    
    data[0] = 0x00; //SPEAKER ATTENUATION OUT 1
    data[1] = 0xC0; //mute TDA7448
    
    uint8_t addr = 0xa0;
    
    while(1) {
        __delay_ms(1000);
        if(!I2C_Send(addr, data, 2)) {
            addr += 2;
            if((addr == 0x94) || (addr == 0x88))
                addr += 2;
        }
        
        // pt2323 test
        //I2C_Send(0x94, data, 1);
        
        //tda7448 test 
        //I2C_Send(0x88, data, 2);        
    };
*/
}

void i2c_stop(void)
{  
    TRISB6 = 0;
    SDA_SET(0);
    TRISB7 = 1;// as input, set Hight (pull up)
    I2C_DELAY(2);
    TRISB6 = 1; // as input, set Hight (pull up)
    I2C_DELAY(2);
}

void i2c_clock(void)
{
    I2C_DELAY(1); //i2cdelay(I2CDATASETTLE);       //-- Minimum Clock Low Time
    TRISB7 = 1;//SCL_TRIS=I2CHIGH;              //-- Release clock
    I2C_DELAY(2); //i2cdelay(I2CCLOCKHIGH);        //-- Minimum Clock High Time
    TRISB7 = 0;//SCL_TRIS=I2CLOW;               //-- Lower the clock
    SCL_DOWN();
    I2C_DELAY(1); //i2cdelay(I2CCLOCKLOW);         //-- Minimum Clock Low Time
}

char i2c_readbit(void)
{
    char Data=0;
    I2C_DELAY(2); //i2cdelay(I2CDATASETTLE);       //-- Minimum Clock Low Time
    TRISB7 = 1;//SCL_TRIS=I2CHIGH;              //-- Release clock
    I2C_DELAY(1); //i2cdelay(I2CHALFCLOCK);        //-- 1/2 Minimum Clock High Time
    if(SDA_GET != 0) Data = 1;//if(SDA !=0 ) Data=1;           //-- READ in the data bit
    I2C_DELAY(1); //i2cdelay(I2CHALFCLOCK);        //-- 1/2 Minimum Clock High Time
    TRISB7 = 0;//SCL_TRIS=I2CLOW;               //-- Lower the clock
    I2C_DELAY(2); //i2cdelay(I2CCLOCKLOW);         //-- Minimum Clock Low Time
return(Data);
}

bool i2c_getack(void)
{
    SDA_SET(0);//SDA=I2CLOW;
    SCL_DOWN();//SCL=I2CLOW;
    TRISB7 = 0;//SCL_TRIS=I2CLOW;                 //-- Ensure clock is low
    TRISB6 = 1;//SDA_TRIS=I2CHIGH;                //-- Release the Data pin so slave can ACK
    TRISB7 = 1;//SCL_TRIS=I2CHIGH;                //-- raise the clock pin
    I2C_DELAY(1);//i2cdelay(I2CHALFCLOCK);          //-- wait for 1/2 the clock pulse
    if(SDA_GET) {                          //-- sample the ACK signal
        TRISB7 = 1;
        return(false);                      //-- No ACK so return BAD
    };
    TRISB7 = 1;
    I2C_DELAY(1);//i2cdelay(I2CHALFCLOCK);          //-- Else wait for rest of clock
    TRISB7 = 0;//SCL_TRIS=I2CLOW;                 //-- Finish the clock pulse
    I2C_DELAY(2);//i2cdelay(I2CCLOCKLOW);           //-- Minimum Clock Low Time
return(true);
}

void i2c_sendack(void)
{
 //--- Send Ack to slave except for last time ---
    SDA_SET(0);//SDA=0;
    TRISB6 = 0;//SDA_TRIS=I2CLOW;              //-- Send ACK
    I2C_DELAY(2);//i2cdelay(I2CDATASETTLE);      //-- Give it time to settle
    i2c_clock();                   //-- Pulse the clock
    TRISB6 = 1;//SDA_TRIS=I2CHIGH;             //-- Release ACK
    I2C_DELAY(2);//i2cdelay(I2CDATASETTLE);      //-- Gap between next byte
}

void i2c_sendbyte(char Byte)
{
    char count;
    SDA_SET(0);//SDA=I2CLOW;
    SCL_DOWN(); //SCL=I2CLOW;

    I2C_DELAY(2);//i2cdelay(I2CCLOCKLOW);         //-- Minimum Clock Low Time

    for(count = 8; count > 0;count--) {  //-- Send 8 bits of data
        if((Byte & 0x80) == 0)  {      //-- Get the Bit
            SDA_SET(0);//SDA=I2CLOW;                  //-- Ensure Port pin is low
            TRISB6 = 0;//SDA_TRIS=I2CLOW;             //-- Lower pin if bit = 0
        } else   {
            TRISB6 = 1;//SDA_TRIS=I2CHIGH;            //-- Release pin if bit = 1
        };
        Byte = Byte<<1;                 //-- Shift next bit into position
        i2c_clock();                   //-- Pulse the clock
    };
    TRISB6 = 1;//SDA_TRIS=I2CHIGH;              //-- Release data pin for ACK
    SDA_SET(0);
}

char i2c_getbyte(void)
{
    char count,Byte=0;
    SDA_SET(0);//SDA=I2CLOW;
    SCL_DOWN(); //SCL=I2CLOW;

    I2C_DELAY(2);//i2cdelay(I2CCLOCKLOW);         //-- Minimum Clock Low Time

    for(count = 8;count > 0; count--) {   //-- Read 8 bits of data
        Byte=Byte <<1;
        TRISB6 = 1;//SDA_TRIS=I2CHIGH;            //-- Release pin so data can be recieved
        if(i2c_readbit()) {
            Byte +=1;
        };
    };
 return(Byte);
}

bool I2C_Read(char Address,char *Data, char Num)
{
    i2c_start();

 //-- Send Address
    i2c_sendbyte(Address | 0x01);   //-- Lowest bit = 1 => READ
 
    if(!i2c_getack())  {
        i2c_stop();
        return(false);
    };

    while(Num--)  {
        *Data=i2c_getbyte();
        Data++;

        if(Num > 0)  {
            i2c_sendack();
        };
    };
    i2c_stop();
return(true);
}

bool I2C_Send(char Address,char *Data,char Num)
{
    i2c_start();

 //-- Send Address
    i2c_sendbyte(Address & 0xFE);   //-- Lowest bit = 0 => WRITE
    if(!i2c_getack()) {
        i2c_stop();
        return(false);
    };

    while(Num--) {
        i2c_sendbyte(*Data);
        if(!i2c_getack()) {
            i2c_stop();
            return(false);
        };
        Data++;
    };
    i2c_stop();
return(true);
}

void i2c_start(void)
{
    i2c_stop();

 //-- Generate the start condition
    TRISB7 = 1;// SCL_TRIS=I2CHIGH; as input, set Hight (pull up)
    TRISB6 = 1;//SDA_TRIS=I2CHIGH; as input, set Hight (pull up)
    I2C_DELAY(1);
    TRISB6 = 0;
    SDA_SET(0);
    I2C_DELAY(1);
    SCL_DOWN();
    TRISB7 = 0;
}