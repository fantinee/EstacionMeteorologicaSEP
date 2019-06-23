/***************************************************
 * This is a I2C library for the ATMega328P.
 *
 * It can provide basic I2C communication for any application.
 *
 * This code is in the public domain. Feel free to do with it whatever you want.
 * 
 * 
 * 
 * FOR STUDENTS:
 * 
 * This file will be given to you in an 'empty' state. The function bodies are
 * provided, but not their content. You are supposed to add the proper code
 * and complete these functions.
 * 
 * Hint 1: RTFM! The Atmel device datasheets contain all information necessary.
 * 
 * Hint 2: We do not expect you to write the most performant or exceptionally
 * well-engineered code, but it should work. If in doubt, sacrifice speed for
 * reliability. If you are bored, rewrite the entire library in assembler and
 * squeeze the last microsecond out of it.
 * 
 ****************************************************/


// includes
#include <avr/io.h>
#include <util/delay.h>
#include "I2C_implement_me.h"

#define SDA_PIN 4
#define SCL_PIN 5

// Send a start condition
void i2cSendStart()
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTA); // Stops the communication
	i2cWaitForComplete();
}




// Send a stop condition
void i2cSendStop()
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); // Stops the communication
}




// Waits for the previous operation to finish
void i2cWaitForComplete()
{
	while (!(TWCR & (1<<TWINT)));
	
}



// Transmits one byte over the bus to a slave
// (Can be either an address or a value.)
void i2cSendByte(unsigned char data)
{
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN); // Starts sending data
	i2cWaitForComplete();
}




// Receives one byte from a slave
// (Always a value; master has no address.)
// Send an ACK if you expect more data, NAK if not.
void i2cReceiveByte(bool sendAnACK)
{
	if(sendAnACK){
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
		
	}else{
		TWCR = (1<<TWINT)|(1<<TWEN);
	}
	i2cWaitForComplete();
}




// After receiving a byte, fetch this byte from the TWDR register
// (if you are interested in it).
// It is possible to *not* call this function after a call to
// i2cReceiveByte() if you are not interested in the actual value.
unsigned char i2cGetReceivedByte(void)
{
	return TWDR;
}