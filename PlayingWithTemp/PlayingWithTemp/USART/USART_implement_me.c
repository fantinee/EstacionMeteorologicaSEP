/***************************************************
 * This is a USART library for the ATMega328P.
 *
 * It can provide basic USART (serial) communication for any application.
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
 * Hint 1: There were several labs about USART communication already. Reusing
 * your code from these tasks might save you a lot of time.
 * 
 * Hint 2: RTFM! The Atmel device datasheets contain all information necessary.
 * 
 * Hint 3: We do not expect you to write the most performant or exceptionally
 * well-engineered code, but it should work. If in doubt, sacrifice speed for
 * reliability. If you are bored, rewrite the entire library in assembler and
 * squeeze the last microsecond out of it.
 * 
 ****************************************************/

#include <avr/io.h>
#include <string.h>

#include "USART_implement_me.h"


// The initialisation function. Call it once from your main() program before
// issuing any USART commands with the functions below!
void USART_Init(uint16_t ubrr)
{
		// Configuration
		int BRC = ubrr;
		UBRR0H = (BRC>>8);
		UBRR0L = (BRC);
		// Enable
		UCSR0B 	= (1<<TXEN0)|(1<<RXEN0);
		// Number of bits
		UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
		// Stop bits
		UCSR0C &=~ (1<<USBS0);
		// No Parity bits
		UCSR0C &=~ (1<<UPM00);
}



// Transmits a single character
void USART_Transmit_char(uint8_t data)
{
	while (!(UCSR0A &(1<<UDRE0)));
	UDR0 = data;
}



// Transmits a given string
void USART_Transmit_String(char* string)
{
	while(*string != 0x00){
		USART_Transmit_char(*string);
		string++;
	}
}



// Receives a single character
char USART_Receive_char(void)
{
	while (!(UCSR0A &(1<<RXC0)));
	return UDR0;
}

void concatenate_string(char *original, char *add)
{
	
	while(*original)
	original++;
	
	while(*add)
	{
		*original = *add;
		add++;
		original++;
	}
	*original = '\0';

}
// Receives a '\n' terminated string and writes it into a supplied buffer.
// The buffer must be guaranteed to handle at least bufflen bytes.
// Returns the number of bytes written into the buffer.
uint8_t USART_Receive_String(char* buffer, uint8_t bufflen)
{
	int index = 0;
	while( index < bufflen){
		char letter = USART_Receive_char();
		buffer[index] = letter;
		index++;
	}
	return (3*bufflen);
} 
