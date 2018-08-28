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

#define I2C_DELAY() __delay_us(200) 
//it is 20k/sec;   PT2323 - max 100k bits / second

void i2c_start(void);
uint8_t i2c_stop(void);
bool i2c_send_byte(uint8_t data);
uint8_t i2c_read_byte(uint8_t ask);
void i2c_restart(void);

char I2C_Read(char Address,char *Data, char Num);
char I2C_Send(char Address,char *Data,char Num);

void i2c_init()
{
    TRISB7 = 0; //set RB7 as output
    TRISB6 = 0; //set RB6 (SDA) as output
    OPTION_REGbits.nRBPU = 1; //PORTB pull up
    
    char data[2];
    data[0] = 0xFF;// Mute on for address 0x94
    __delay_ms(500);
    
    while(1) {
        __delay_ms(1000);
//        I2C_Read(0xA0, data, 2);
        I2C_Send(0x94, data, 1);
    };

//    SCL_DOWN();
//    SDA_SET(1);
//    __delay_ms(1000);
//    
//    SCL_UP();
//    __delay_ms(300);
//    SCL_DOWN();
//    __delay_ms(110);
//
//    while(1) {
//    
//    __delay_ms(1000);
//        
//    // Start
//    SCL_UP();
//    __delay_ms(1);
//    SDA_SET(0);
//    I2C_DELAY();
//    SCL_DOWN();
//    I2C_DELAY();
//    SDA_SET(1);
//    I2C_DELAY();
//    I2C_DELAY();
//    
//    //Address set
//    uint8_t data = 0xA0;
//    for(uint8_t i = 0; i < 8; i++) {
//		if((data & 0x80) == 0) {
//			SDA_SET(0);
//		} else {
//			SDA_SET(1);
//		}
//		I2C_DELAY();
//        SCL_UP();
//        I2C_DELAY();
//		SCL_DOWN();
//        
//		data = (data << 1);
//	};
//    SDA_SET(0);
//        I2C_DELAY();
//    SDA_IN();
//    I2C_DELAY();
//    SCL_UP();
//    I2C_DELAY();
//    if(SDA_GET == 0) {while(1) {};};
//    SCL_DOWN();
//    SDA_OUT();
//    
//    I2C_DELAY(); I2C_DELAY();
//    I2C_DELAY(); I2C_DELAY();
//    SDA_IN();
//    
//    //Data read
//    uint8_t byte = 0;
//    for(uint8_t i = 0; i < 8; i++) {
//		byte = (byte << 1);
//        SCL_UP();//		ONE_SCL();
//        I2C_DELAY();
//        if(SDA_GET == 1) byte |= 0x01;
//        SCL_DOWN();
//        I2C_DELAY();
//	};
//    SDA_SET(0);
//        I2C_DELAY();
//    SDA_IN();
//    I2C_DELAY();
//    SCL_UP();
//    I2C_DELAY();
//    SCL_DOWN();
//    SDA_OUT();
//
//    
//    //Stop 
//    SCL_DOWN();
//    I2C_DELAY();
//    SDA_SET(1);
//    I2C_DELAY();
//    SDA_SET(0);
//    I2C_DELAY();
//    SCL_UP();
//    I2C_DELAY();
//    SDA_SET(1);
//    I2C_DELAY();
//    
//    SCL_UP();//SCL_DOWN();    
//    };
    
    /*
    SCL_UP();
    
    SDA_SET(1);
    
    i2c_stop();
 //   i2c_reset();
    __delay_ms(5);
    
    uint8_t ad = 0x00;
    
    while(1) {
     //   i2c_reset();
        i2c_start();
        ad++;
        if(i2c_send_byte(ad)) break;// eeprom read; 0xA1
    //    __delay_ms(5);
    //    volatile uint8_t rb = i2c_read_byte(NACK); // read byte
        i2c_stop();
        __delay_ms(500);
    };
    while(1) {};
    */
}

uint8_t i2c_stop(void)
{
    uint8_t error = 0;
//    /*
//    SDA_OUT();
//    SDA_SET(0);
//    SCL_UP();
//    I2C_DELAY();
//    SDA_SET(1);
//    */
//    
//    
//    SCL_DOWN();
//    I2C_DELAY();
//    SDA_OUT();
//    SDA_SET(0);
//    I2C_DELAY();
//    
//    SCL_UP();
//    I2C_DELAY();
//    SDA_SET(1);
//    I2C_DELAY();
//    
//		
//	SDA_IN();
//    TRISB7 = 1; // SCL as input;
//
//    if(SDA_GET == 0) error = 2;
//    if(PORTBbits.RB7 == 0) error |= 1;
//    
//    TRISB7 = 0; // SCL as output;
//    I2C_DELAY();
    		
  //-- Generate Stop Condition --
/*
 TRISB6 = 0;//SDA_TRIS=I2CLOW;
 TRISB7 = 1;//SCL_TRIS=I2CHIGH;
 I2C_DELAY(); //i2cdelay(I2CSTOPDELAY);
 TRISB6 = 1;//SDA_TRIS=I2CHIGH;
*/
  
 TRISB6 = 0;
 SDA_SET(0);
 TRISB7 = 1;// as input, set Hight (pull up)
 I2C_DELAY();
 TRISB6 = 1; // as input, set Hight (pull up)
 I2C_DELAY();

return error;
}

void i2c_clock(void)
{
    I2C_DELAY(); //i2cdelay(I2CDATASETTLE);       //-- Minimum Clock Low Time
    TRISB7 = 1;//SCL_TRIS=I2CHIGH;              //-- Release clock
    I2C_DELAY(); //i2cdelay(I2CCLOCKHIGH);        //-- Minimum Clock High Time
    TRISB7 = 0;//SCL_TRIS=I2CLOW;               //-- Lower the clock
    SCL_DOWN();
    I2C_DELAY(); //i2cdelay(I2CCLOCKLOW);         //-- Minimum Clock Low Time
}

char i2c_readbit(void)
{
 char Data=0;
 I2C_DELAY(); //i2cdelay(I2CDATASETTLE);       //-- Minimum Clock Low Time
 TRISB7 = 1;//SCL_TRIS=I2CHIGH;              //-- Release clock
 I2C_DELAY(); //i2cdelay(I2CHALFCLOCK);        //-- 1/2 Minimum Clock High Time
 if(SDA_GET != 0) Data = 1;//if(SDA !=0 ) Data=1;           //-- READ in the data bit
 I2C_DELAY(); //i2cdelay(I2CHALFCLOCK);        //-- 1/2 Minimum Clock High Time
 TRISB7 = 0;//SCL_TRIS=I2CLOW;               //-- Lower the clock
 I2C_DELAY(); //i2cdelay(I2CCLOCKLOW);         //-- Minimum Clock Low Time
 return(Data);
}

char i2c_getack(void)
{
 SDA_SET(0);//SDA=I2CLOW;
 SCL_DOWN();//SCL=I2CLOW;
 TRISB7 = 0;//SCL_TRIS=I2CLOW;                 //-- Ensure clock is low
 TRISB6 = 1;//SDA_TRIS=I2CHIGH;                //-- Release the Data pin so slave can ACK
 TRISB7 = 1;//SCL_TRIS=I2CHIGH;                //-- raise the clock pin
 I2C_DELAY();//i2cdelay(I2CHALFCLOCK);          //-- wait for 1/2 the clock pulse
//__delay_ms(7);
if(SDA_GET) {                          //-- sample the ACK signal
    TRISB7 = 1;
    return(0);                      //-- No ACK so return BAD
};
TRISB7 = 1;
 I2C_DELAY();//i2cdelay(I2CHALFCLOCK);          //-- Else wait for rest of clock
 TRISB7 = 0;//SCL_TRIS=I2CLOW;                 //-- Finish the clock pulse
 I2C_DELAY();//i2cdelay(I2CCLOCKLOW);           //-- Minimum Clock Low Time
 I2C_DELAY();//i2cdelay(I2CCLOCKLOW);           //-- Minimum Clock Low Time
 return(1);
}

void i2c_sendack(void)
{
 //--- Send Ack to slave except for last time ---
 SDA_SET(0);//SDA=0;
 TRISB6 = 0;//SDA_TRIS=I2CLOW;              //-- Send ACK
 I2C_DELAY();//i2cdelay(I2CDATASETTLE);      //-- Give it time to settle
 i2c_clock();                   //-- Pulse the clock
 TRISB6 = 1;//SDA_TRIS=I2CHIGH;             //-- Release ACK
 I2C_DELAY();//i2cdelay(I2CDATASETTLE);      //-- Gap between next byte
}

char i2c_sendbyte(char Byte)
{
 char count;
 SDA_SET(0);//SDA=I2CLOW;
 SCL_DOWN(); //SCL=I2CLOW;

 I2C_DELAY();//i2cdelay(I2CCLOCKLOW);         //-- Minimum Clock Low Time

for(count = 8; count > 0;count--) {  //-- Send 8 bits of data
    if((Byte & 0x80) == 0)  {      //-- Get the Bit
        SDA_SET(0);//SDA=I2CLOW;                  //-- Ensure Port pin is low
        TRISB6 = 0;//SDA_TRIS=I2CLOW;             //-- Lower pin if bit = 0
    } else   {
        TRISB6 = 1;//SDA_TRIS=I2CHIGH;            //-- Release pin if bit = 1
    }
    Byte = Byte<<1;                 //-- Shift next bit into position
    i2c_clock();                   //-- Pulse the clock
 };
 TRISB6 = 1;//SDA_TRIS=I2CHIGH;              //-- Release data pin for ACK
 SDA_SET(0);
 return(1);
}

char i2c_getbyte(void)
{
 char count,Byte=0;
 SDA_SET(0);//SDA=I2CLOW;
 SCL_DOWN(); //SCL=I2CLOW;

 I2C_DELAY();//i2cdelay(I2CCLOCKLOW);         //-- Minimum Clock Low Time

 for(count=8;count>0;count--)   //-- Read 8 bits of data
 {
  Byte=Byte <<1;
  TRISB6 = 1;//SDA_TRIS=I2CHIGH;            //-- Release pin so data can be recieved
  if(i2c_readbit())
  {
   Byte +=1;
  }
 }
 return(Byte);
}


char I2C_Read(char Address,char *Data, char Num)
{
 i2c_start();

 //-- Send Address
 i2c_sendbyte(Address | 0x01);   //-- Lowest bit = 1 => READ
 
 if(!i2c_getack())  {
  i2c_stop();
  return(0);
 }

 while(Num--)  {
  *Data=i2c_getbyte();
  Data++;

  if(Num > 0)  {
   i2c_sendack();
  }
 }
 i2c_stop();
 return(1);
}

char I2C_Send(char Address,char *Data,char Num)
{
i2c_start();

 //-- Send Address
 i2c_sendbyte(Address & 0xFE);   //-- Lowest bit = 0 => WRITE
if(!i2c_getack()) {
    i2c_stop();
    return(0);
};

while(Num--) {
    i2c_sendbyte(*Data);
    if(!i2c_getack()) {
        i2c_stop();
        return(0);
    };
    Data++;
};
i2c_stop();
return(1);
}




void i2c_start(void)
{
// TRIS &= 0b11111001;	 /* make SCL and SDA outputs 	*/
// SDA = 0;
// ee_delay();
// SCL = 1;
// ee_delay();
// SDA = 1;
// ee_delay();             /* Stop condition */
//
// SDA=0;
// ee_delay();             /* Start Condition */

 /*
   //-- Ensure pins are in high impedance mode --
 TRISB7 = 1;// SCL_TRIS=I2CHIGH; as input, set Hight (pull up)
 TRISB6 = 1;//SDA_TRIS=I2CHIGH; as input, set Hight (pull up)
 SCL_DOWN(); //SCL=I2CLOW;
 SDA_SET(0);//SDA=I2CLOW;
 I2C_DELAY(); //i2cdelay(I2CSTARTDELAY);
 */
 
 //Stop condition
    /*
 TRISB6 = 0;
 SDA_SET(0);
 TRISB7 = 1;// as input, set Hight (pull up)
 I2C_DELAY();
 TRISB6 = 1; // as input, set Hight (pull up)
 I2C_DELAY();
 */
 i2c_stop();

 //-- Generate the start condition
 TRISB7 = 1;// SCL_TRIS=I2CHIGH; as input, set Hight (pull up)
 TRISB6 = 1;//SDA_TRIS=I2CHIGH; as input, set Hight (pull up)
 I2C_DELAY();
 TRISB6 = 0;
 SDA_SET(0);
 I2C_DELAY();
 SCL_DOWN();
 TRISB7 = 0;
 
 /*
 TRISB6 = 0;//SDA_TRIS=I2CLOW; as output
 SDA_SET(0);//SDA=I2CLOW;
 I2C_DELAY(); //i2cdelay(I2CSTARTDELAY);
 TRISB7 = 0;//SCL_TRIS=I2CLOW; set as output
 I2C_DELAY(); //i2cdelay(I2CCLOCKLOW);         //-- Minimum Clock Low Time
 */
 __delay_ms(4);
}

void i2c_restart(void)
{
    SDA_OUT();
    SDA_SET(1);
    I2C_DELAY();
    SCL_UP();
	I2C_DELAY();
		
    SDA_SET(0);
    I2C_DELAY();
    SCL_DOWN();
	I2C_DELAY();
}

void i2c_reset()
{
    i2c_start();
    
    for(uint8_t kx = 0; kx < 9; kx++) {
        SCL_DOWN();
        I2C_DELAY();
        SCL_UP();
        I2C_DELAY();
    };
    
    SCL_DOWN();
    i2c_start();
    i2c_stop();
}

bool i2c_send_byte(uint8_t data)
{
	bool ask = true;
    
	SCL_DOWN();
    SDA_OUT();
    I2C_DELAY();
		
	for(uint8_t i = 0; i < 8; i++) {
		if((data & 0x80) == 0) {
			SDA_SET(0);
		} else {
			SDA_SET(1);
		}
		I2C_DELAY();
        SCL_UP();
        I2C_DELAY();
		SCL_DOWN();
        
		data = (data << 1);
	}
			
	SDA_SET(1);
    SDA_IN();
	I2C_DELAY();
	SCL_UP();
	I2C_DELAY();
    
 //   while(SDA_GET == 1) {}; //TODO:
    
    if(SDA_GET == 1) {
        ask = false;
    } else {
        ask = true;
    }
		
	SCL_DOWN();
    I2C_DELAY();
			
return ask;
}

uint8_t i2c_read_byte(uint8_t ask)
{
	uint8_t byte = 0;
	
    SDA_OUT();	
	SDA_SET(1);
    SDA_IN();
		
	for(uint8_t i = 0; i < 8; i++) {
		byte = (byte << 1);
        SCL_UP();//		ONE_SCL();
        I2C_DELAY();
        if(SDA_GET == 1) byte |= 0x01;
        SCL_DOWN();
        I2C_DELAY();
	};
			
    SDA_OUT();
    if(ask == ACK)	{
		SDA_SET(0);
	} else {
		SDA_SET(1);
	};
    
    I2C_DELAY();
    SCL_UP();//	ONE_SCL();
	I2C_DELAY();
	SCL_DOWN();//	NULL_SCL();
//	I2C_DELAY();
	SDA_SET(1);//	ONE_SDA();
			
return byte;
}

bool i2c_block_write(uint8_t address, uint8_t *data, uint8_t len, bool stop)
{
   i2c_start();
    
    address &= ~0x01;
    if(!i2c_send_byte(address)) return false;
    
    for(uint8_t kx = 0; kx < len; kx++) {
        if(!i2c_send_byte(*data)) return false;
        data++;
    };   
    
    if(stop) {
        i2c_stop();
    }
    __delay_ms(5);
    
return true;
}

bool i2c_block_read(uint8_t address, uint8_t *data, uint8_t len)
{
    i2c_start();
    
    address |= 0x01;
    if(!i2c_send_byte(address)) return false;
    
    for(uint8_t kx = 0; kx < len - 1; kx++) {
        *data = i2c_read_byte(ACK);
        data++;
    };
    *data = i2c_read_byte(NACK);
    
    i2c_stop();
    
return true;
}

bool i2c_data_read(uint8_t device_address, uint8_t data_address, uint8_t *data, uint8_t len)
{
    uint8_t addr_dat = data_address;
    
    if(!i2c_block_write(device_address, &addr_dat, 1, false)) {
        i2c_stop();
        i2c_reset();
        return false;
    }
    
    if(!i2c_block_read(device_address, data, len)) {
        i2c_reset();
        return false;
    }
    
return true;
}