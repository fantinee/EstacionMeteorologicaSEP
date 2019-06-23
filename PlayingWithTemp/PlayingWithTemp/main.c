/*
 * PlayingWithTemp.c
 *
 * Created: 02-06-2019 18:54:14
 * Author : Melanie
 */ 

#define F_CPU	16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avr/interrupt.h>

int aux = 0;

#include "USART/USART_implement_me.h"
#include "SPI/SPI_implement_me.h"
#include "display/ST7735_commands.h"
#include "display/graphic_shapes.h"
// read and write addresses; set ADD0 = GND
#define TMP102_READ	 	0x91
#define TMP102_WRITE 	0x90
// UART configuration
#define BAUD	57600					// serial communication baud rate
#define UBRR_VALUE F_CPU/16/BAUD-1

int16_t tmp102Read(void)
{
	
	uint8_t msb, lsb;
	int16_t temp;
	
		
	i2cSendStart();							// send start condition
	i2cWaitForComplete();
	
	i2cSendByte(TMP102_WRITE); 				// send WRITE address of TMP102
	i2cWaitForComplete();
	i2cSendByte(0x00); 						// set TMP102 pointer register to 0 (read temperature)
	i2cWaitForComplete();
	
	i2cSendStart();							// send repeated start condition
	i2cWaitForComplete();
	
	i2cSendByte(TMP102_READ);				// send READ address of TMP102
	i2cWaitForComplete();
	i2cReceiveByte(true);					// receives one byte from the bus and ACKs it
	i2cWaitForComplete();
	msb = i2cGetReceivedByte(); 			// reads the MSB (it is a 12 bit value!)
	i2cWaitForComplete();
	i2cReceiveByte(false);					// receives one byte from the bus and NAKs it (last one)
	i2cWaitForComplete();
	lsb = i2cGetReceivedByte();				// reads the LSB
	i2cWaitForComplete();
	
	i2cSendStop();							// send stop condition
	TWCR = 0;								// stop everything
	
	
	// Convert the number to an 8-bit degree Celsius value
	temp = (msb<<8) | lsb;					// combine those two values into one 16 bit value
	temp >>= 4; 							// the values are left justified; fix that by shifting 4 right
	// negative numbers are represented in two's complement, but we just shifted the value and thereby potentially messed it up
	if(temp & (1<<11))						// Hence: if it is a negative number
		temp |= 0xF800;						// restore the uppermost bits
		
	// The 12 bit value has 0.0625°C precision, which is too much for what we want (and the sensor is anyways only about 0.5°C precise)
	// 0.0625 is 1/16 -> Dividing by 16 leaves 1°C precision for the output. Note that shifting >> 4 might go wrong here for negative numbers.
	temp /= 16;
	
	return(temp);
}

#define PI 3.1415926535897932384626433832795

int16_t tempData [10] = {23,12,21,21,20,28,20,10,15,20};
int16_t chartXpos = 0;
int main(void)
{
	// constantes
	int a=1000,b=3500;
	int n, f;
	int j, j2 ,lj;
	int i, i2 ,li;
	int pct = 0;
	int d[5] = {20, 20, 20, 20, 20};
	uint16_t col[5] = {0x7006, 0xF986, 0x6905, 0x7FF7, 0x024D};
	//init cosas
	USART_Init(UBRR_VALUE);	
	SPI_Master_Init();
	ST7735_init();
	I2C_INIT();
	USART_Transmit_String("Partio el programa");
	fillScreen(ST7735_WHITE);
	_delay_ms(100);
    /* Replace with your application code */
	draw_thermometer();
	//draw_plot();

	//newData(42);
	//newData(66);
	//newData(48);
	
	
    while (1)
    {

			int16_t temperature = 0;
			temperature = tmp102Read();
			USART_Transmit_String("Temperature: ");
			int aux_y = 90-2*temperature;
			//newData(aux_y);
			char buffer[10];
			itoa(temperature, buffer, 10);
			USART_Transmit_String(buffer);
			drawtext(80,50,buffer,ST7735_BLUE,ST7735_WHITE,5);
			drawtext(80,90,"deg. Celsius",ST7735_BLUE,ST7735_WHITE,1);
			//USART_Transmit_String(" deg. Celsius\r\n");
			


			//drawtext(20,105,"Max: ",ST7735_BLUE,ST7735_WHITE,1);
			//maxValue();
			//drawtext(40,105,maxValue(),ST7735_BLUE,ST7735_WHITE,1);
			//drawtext(70,105,"Min: ",ST7735_BLUE,ST7735_WHITE,1);
			//drawtext(120,105,"AVG: ",ST7735_BLUE,ST7735_WHITE,1);
			
	}
}

void I2C_INIT(){
		// I2C initialisation
		TWBR = 12;									// 400 kHz at 16MHz crystal and no prescaler
		TWSR &= ~((1 << TWPS1) | (1 << TWPS0));		// prescaler 0
		TWDR = 0xFF;								// sending only ones equals an idle high SDA line
		TWCR = (1<<TWEN)|							// Enable TWI-interface and release TWI pins.
		(0<<TWIE)|(0<<TWINT)|				// Disable Interupt.
		(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|	// No Signal requests.
		(0<<TWWC);	
		sei();								//
}

void draw_thermometer(){
		fillCircle(40,90,25,ST7735_LIGHTBLUE);
		fillCircle(40,90,18,ST7735_RED);
		fillCircle(39,17,9,ST7735_LIGHTBLUE);
		fillRect(30,20,20,52,ST7735_LIGHTBLUE);
		fillRect(35,20,10,52,ST7735_RED);
		drawLine(35,30,38,30,ST7735_BLACK);
		drawLine(36,30,38,30,ST7735_BLACK);
		
		drawLine(35,35,38,35,ST7735_BLACK);
		drawLine(35,36,38,36,ST7735_BLACK);
		
		drawLine(35,40,38,40,ST7735_BLACK);
		drawLine(35,44,38,44,ST7735_BLACK);
		drawLine(35,45,38,45,ST7735_BLACK);

		drawLine(35,50,38,50,ST7735_BLACK);
		drawLine(35,54,38,54,ST7735_BLACK);
		drawLine(35,55,38,55,ST7735_BLACK);
}

void draw_plot(){
	drawLine(15,90,145,90,ST7735_BLACK);
	drawLine(15,90,15,10,ST7735_BLACK);
	// Ref values
	drawLine(8,70,20,70,ST7735_BLACK);
	drawtext(2,70,"10",ST7735_BLACK,ST7735_WHITE,1);
	drawLine(8,50,20,50,ST7735_BLACK);
	drawtext(2,50,"20",ST7735_BLACK,ST7735_WHITE,1);
	drawLine(8,30,20,30,ST7735_BLACK);
	drawtext(2,30,"30",ST7735_BLACK,ST7735_WHITE,1);
}

void newData(int chartYpos){
	_delay_ms(1000);
	///int chartYpos = 90 - (90-30)*(value/30);
	USART_Transmit_String(chartYpos);
	if (chartXpos==120){
		fillScreen(ST7735_WHITE);
		draw_plot();
		chartXpos = 0;
	}
	//draw new point
	drawCircle(15+chartXpos, chartYpos,4,ST7735_DOT_BLUE);
	drawCircle(15+chartXpos, chartYpos,3,ST7735_DOT_BLUE);
	fillCircle(15+chartXpos, chartYpos,2,ST7735_DOT_BLUE);
	
	// draw line from last point
	
	// paint everything under the graph
	chartXpos = chartXpos + 12 ;
}

void maxValue(){
	char buffer0[10];
	int index = 1;
	int maxvalue = tempData[0];
	while(index<10){
	if (tempData[index]>maxValue){
		maxvalue = tempData[index];
	}
	index ++;
	}
	char digit1 = maxvalue/10+'0';
	char digit2 = (maxvalue-maxvalue/10)+'0';
	drawtext(40,105,digit1,ST7735_BLUE,ST7735_WHITE,1);
	drawtext(40,115,digit2,ST7735_BLUE,ST7735_WHITE,1);
//	return buffer0;
	
}
void minValue(){
		int index = 1;
		int minvalue = tempData[0];
		if (tempData[index]<maxValue){
			minvalue = tempData[index];
		}
		index ++;
		if (index == 9){
			return minvalue;
		}
}

void avgValue(){
	float total;
	for(int i; i<10;i++){
		total = total + tempData[i];
		}
	return total/10;
}