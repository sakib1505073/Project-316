// #define F_CPU 1000000
// #include <avr/io.h>
// #include <avr/interrupt.h>
// #include <util/delay.h>


// void UART_init(){
// 	UCSRA = 0x00 ;
// 	UCSRB = 0x18 ;
// 	UCSRC = 0x86 ;
// 	UBRRL = 0x33 ;
// 	UBRRH = 0x00 ;
// }

// void UART_send(unsigned char data){
// 	while((UCSRA & (1<<UDRE))==0x00);
// 	UDR = data ;
// }

// unsigned char UART_receive(){
// 	while(((UCSRA)&(1<<RXC))==0);
// 	return UDR ;
// }


// void SPI_SlaveInit(void)
// {
// /* Set MISO output, all others input */
// DDRB = (1<<DDB6);
// /* Enable SPI */
// SPCR = (1<<SPE);
// }

// char SPI_SlaveReceive(void)
// {
// /* Wait for reception complete */
// while(!(SPSR & (1<<SPIF)));
// /* Return data register */
// return SPDR;
// }

// int main(void)
// {
// 	DDRD = 0xFF ;
	
// 	SPI_SlaveInit();
// 	unsigned char c  ;
	
//     while(1)
//     {
// 		c = SPI_SlaveReceive() ;
// 		if(c=='1'){
// 			PORTD = 0xFF ;
// 			_delay_ms(250) ;
// 			PORTD = 0x00 ;
// 			_delay_ms(250) ;
// 		}
//         c=0;
//     }
// }









/*
 * SerialCom1.c
 *
 * Created: 5/25/2018 11:50:26 AM
 *  Author: Spondon
 */ 

#define F_CPU 1000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void UART_init(){
	UCSRA = 0x00 ;
	UCSRB = 0x18 ;
	UCSRC = 0x86 ;
	UBRRL = 0x33 ;
	UBRRH = 0x00 ;
}

void UART_send(unsigned char data){
	while((UCSRA & (1<<UDRE))==0x00);
	UDR = data ;
}

unsigned char UART_receive(){
	while(((UCSRA)&(1<<RXC))==0);
	return UDR ;
}

ISR(INT1_vect){
	UART_send('a') ;
	UCSRA = 0x00 ;
	PORTA = 0xFF;
	_delay_ms(100) ;
	PORTA = 0x00 ;
	_delay_ms(100) ;
}
int main(void)
{
	DDRD = 0xFF ;
	DDRA = 0xFF ;
	UART_init() ;
	unsigned char c ;
	MCUCR=0x0C ;
	GICR =(1<<INT1) ;
	sei() ;
    while(1)
    {
		c = UART_receive() ;
		if(c=='a'){
			PORTD = 0xFF ;
			_delay_ms(250) ;
			PORTD= 0x00 ;
			_delay_ms(250) ;
		}
    }
}