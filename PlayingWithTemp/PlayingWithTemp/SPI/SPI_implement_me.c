/***************************************************
 * This is a SPI library for the ATMega328P.
 *
 * It can provide basic SPI master functionality for any application, but is
 * directly targeted at the Adafruit 1.8" SPI display and its SD card socket.
 * 
 * It is based on example codes from the Atmel device datasheet. The code is
 * kept simple, but is not always efficient. It is an easy-to-understand example
 * for studies and not intended for productive use.
 *
 * This code is in the public domain. Feel free to do with it whatever you want.
 * 
 * 
 * 
 * FOR STUDENTS:
 * 
 * This file will be given to you in an 'empty' state. The function bodies are
 * provided, but not their content. You are supposed to add the proper setup
 * code and complete these functions.
 * 
 * Hint 1: RTFM! The Atmel device datasheets contain all information necessary.
 * 
 ****************************************************/

#include <avr/io.h>

#include "SPI_implement_me.h"


// The initialisation function. Call it once from your main() program before
// issuing any SPI commands with the functions below!
//
// Hint: It shoud activate the according SPI module in the microcontroller and
// probably set some register values. SPI has many options to configure! Read
// about them and find out, which ones are to be used here. Understanding the
// datasheets will be much faster than a simple try and error approach.
//
// Also note that <avr/io.h> and other files provide convenient macros for most
// registers and their bits. Stuff like
//    EXPL = (1<<POS)|(1<<FOO);
// is much nicer to read than
//    EXPL = 0b10010;
// This is not a requirement, but it will make life easier for you and your
// supervisors.
void SPI_Master_Init(void)
{
    // Set MOSI, SCK as Output
	// Init PB1 = CARD_CS PB2 = TFT_CS  PB0 = D/C
    DDRB |= (1<<5)|(1<<3)|(1<<2)|(1<<1)|(1<<0);
	DDRB &= ~(1<<4); 
    // Enable SPI, Set as Master
    //Prescaler: Fosc/16, Enable Interrupts
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	//Star with the card_cs 
	PORTB |= (1 << DDB2);
	PORTB &= ~(1 << DDB1);
}


// This function transmits a single byte over the SPI bus.
//
// It does *not* control the CS line.
void SPI_Master_transmit_char(uint8_t data)
{
	SPDR = data; // initiate transfer
	while (!(SPSR & (1<<SPIF))); // wait for transfer to complete
}

void SPI_Master_transmit_charTFT(uint8_t data, bool commandmode)
{
	// PB2 ->TFT on every time we are not sending anything
	// PB0 -> DC
	PORTB &= ~ (1 << DDB2); // TFT OFF
	PORTB |= (1 << DDB1);  // CARD_CS 1
	if (commandmode){//sending a command
		// DC off
		PORTB &= ~ (1<<DDB0);
		SPI_Master_transmit_char(data);
	}else{
		//tft off / DC on
		PORTB |= (1<<DDB0);
		SPI_Master_transmit_char(data);
	}
	PORTB |= (1 << DDB2);   // TFT ON
	PORTB &= ~ (1 << DDB1); // CARD_CS 0
}

// This function receives a single byte over the SPI bus.
//
// This is very easy and short if you understood how SPI works.
// Hint: It is a *full duplex* bus!
char SPI_Master_receive_char(void)
{
	SPI_Master_transmit_char(0xFF);
	while (!(SPSR & (1<<SPIF))); // wait for transfer to complete
	return SPDR;
}