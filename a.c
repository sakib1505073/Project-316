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
	while(((UCSRA)&(1<<RXC))==0x00);
	return UDR ;
}

ISR(INT1_vect) //STEP2
{
    
        
    SPI_MasterTransmit('1') ;
	UCSRA = 0x00 ;
	PORTA = 0xFF;
	_delay_ms(100) ;
	PORTA = 0x00 ;
	_delay_ms(100) ;
    GIFR=0xFF;
}

void SPI_MasterInit(void)
{
/* Set MOSI and SCK output, all others
input */
DDRB = (1<<DDB5)|(1<<DDB7);
/* Enable SPI, Master, set clock rate
fck/16 */
SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_MasterTransmit(char cData)
{
/* Start transmission */
SPDR = cData;
/* Wait for transmission complete */
while(!(SPSR & (1<<SPIF)));
}

int main(void)
{
	DDRB = 0xFF ;
	DDRA = 0xFF ;
    PORTA=0;
    DDRD=0x00;
    PORTD=0x00;
	SPI_MasterInit() ;
	unsigned char c  ;
	MCUCR = 0x0C;

	GICR =(1<<INT1)  ;
	sei() ;
    while(1)
    {
		// c = UART_receive() ;
		// if(c=='1'){
		// 	PORTB = 0xFF ;
		// 	_delay_ms(250) ;
		// 	PORTB = 0x00 ;
		// 	_delay_ms(250) ;
		// }
        // c=0;
    }
}