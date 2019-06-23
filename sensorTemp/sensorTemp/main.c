#define F_CPU 16000000UL
#define F_CPU 16000000UL
#define BAUD 57600
#define BRC (F_CPU/16/BAUD)-1
#include <avr/io.h>
#include <util/delay.h>
#include "DHT/DHT.h"
#include "USART/USART_implement_me.h"


int main(void)
{
	//Variables
	double temp[1], hum[1];		//Return values of sensor - Must be pointers or more preferably size-one arrays

	//Setup
	DHT_Setup();

	USART_Init(BRC);
	
	//Loop
	while (1 == 1)
	{
		//Read from sensor
		DHT_Read(temp, hum);
		
		int temperatura = temp[0]*100;
		int humedad = hum[0]*100;
		
		//Check status
		switch (DHT_status())
		{
			case (DHT_Ok):
			//Print temperature
			//print(temp[0]);
			USART_Transmit_String("Temperatura: ");
			USART_Transmit_char((int)((temperatura/1000) + 48));
			USART_Transmit_char((int)((temperatura%1000)/100 + 48));
			USART_Transmit_String(".");
			USART_Transmit_char((int)((temperatura%100)/10 + 48));
			USART_Transmit_char((int)((temperatura%10) + 48));
			USART_Transmit_String("    ");
			USART_Transmit_String("Humedad: ");
			USART_Transmit_char((int)((humedad/1000) + 48));
			USART_Transmit_char((int)((humedad%1000)/100 + 48));
			USART_Transmit_String(".");
			USART_Transmit_char((int)((humedad%100)/10 + 48));
			USART_Transmit_char((int)((humedad%10) + 48));
			USART_Transmit_String("%\r\n");
			
			//Print humidity
			//print(hum[0]);
			break;
			case (DHT_Error_Checksum):
			USART_Transmit_String("error check sum");
			break;
			case (DHT_Error_Timeout):
			USART_Transmit_String("error timeout");
			break;
			default:
			USART_Transmit_String("otracosa");
			break;
		}
		
		//Sensor needs 1-2s to stabilize its readings
		_delay_ms(3000);
	}
	
	return 0;
}