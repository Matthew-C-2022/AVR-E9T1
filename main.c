_CPU 20E6
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
#define BAUD_VAL 129
char ADC_String[20];
volatile int ADC_Result = 30;
#define compare 22753

volatile uint8_t RXFlag = 0; 
volatile int i = 0;
char buf[22];
volatile char temp;
char printsend = 0;
void TX_Character(char Ch){
	
	while((UCSR0A & 1<<UDRE0) == 0);
	
	UDR0 = Ch;		//Transmit '1'
	
}

void Tx_String(char*String){
	for(int i = 0; i < strlen(String); i++)
	{
		TX_Character(String[i]);
	}
	TX_Character('\r');
}


char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)));
	/* Get and return received data from buffer */
	
	return (char)UDR0;
}


int main(void)
{
	
	// ADC Initialisation
	ADMUX = 1<<REFS0 | 1<<MUX1;
	ADCSRA = 1<<ADEN | 1<<ADPS1;
	DIDR0 = 0xFF;
	UCSR0B = 1<<TXEN0 | 1<<RXEN0 | 1<<RXCIE0;
	OCR1A = compare; // Load compare value
	TCCR1B = 1<<WGM12 | 1<<CS10 | 1<<CS12;
	TIMSK1 = 1<<OCIE1A;
	sei();
	UBRR0 = BAUD_VAL;//Sets UBRR0 to the BAUD_VAL for 20E6MHZ

	DDRB |= 1<<PB3 | 1<<PB4;
	DDRD |= 1<<PD4 | 1<<PD5;

	

    while (1) 
    {

		if(buf[i] == ';')
		{
			printsend = 1;
		}
		
		
		
		
		
		if(printsend)
		{
			i++;
			buf[i] = '\0';
			sprintf(ADC_String, "RECIVED BYTE: %s I = %d RECVSTATE = %d",buf, i, (UCSR0A & 1<<RXC0));
			Tx_String(ADC_String);
			i = 0;
			RXFlag = 0;
			printsend = 0;
		}
		
		
    }
}

ISR(USART0_RX_vect)
{

	temp = (char)UDR0;
	buf[i] = temp;
	i++;
	if((char)UDR0 == 'R')
	{
		PORTB ^= 1<<PD3;
	}
	
	else if((char)UDR0 == 'r')
	{
		PORTB &= ~(1<<PD3);
	}


	if((char)UDR0 == 'G')
	{
		PORTB ^= 1<<PD4;
	}
	
	else if((char)UDR0 == 'g')
	{
		PORTB &= ~(1<<PD4);
	}
	
	
	if((char)UDR0 == 'B')
	{
		PORTD ^= 1<<PD4;
	}
	
	else if((char)UDR0 == 'b')
	{
		PORTD &= ~(1<<PD4);
	}


	if((char)UDR0 == 'Y')
	{
		PORTD ^= 1<<PD5;
	}
	
	else if((char)UDR0 == 'y')
	{
		PORTD &= ~(1<<PD5);
	}
}
