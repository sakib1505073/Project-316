#define F_CPU 1000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
/* Wait for completion of previous write */
while(EECR & (1<<EEWE));
/* Set up address and data registers */
EEAR = uiAddress;
EEDR = ucData;
/* Write logical one to EEMWE */
EECR |= (1<<EEMWE);
/* Start eeprom write by setting EEWE */
EECR |= (1<<EEWE);
}

unsigned char EEPROM_read(unsigned int uiAddress)
{
/* Wait for completion of previous write */
while(EECR & (1<<EEWE));
/* Set up address register */
EEAR = uiAddress;
/* Start eeprom read by writing EERE */
EECR |= (1<<EERE);
/* Return data from data register */
return EEDR;
}


int main(void)
{
    DDRB=0xFF;
    PORTB=0x00;
unsigned int uiAddress=0;
unsigned char c;


for (uiAddress=0;uiAddress<26;uiAddress++)
{
EEPROM_write(uiAddress,uiAddress+97);
_delay_ms(200);
}
while(1){
    c = EEPROM_read(0);
if(c=='a')PORTB=0xff;
else PORTB=0x00;
}
}
