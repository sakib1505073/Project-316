#define F_CPU 125000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "LCD_16x2_C_file.c"/* Include LCD header file */


#define  Trigger_pin	PA0	/* Trigger pin */

volatile int TimerOverflow = 0;
volatile double distance;
volatile long count;
volatile int mode=0;
ISR(TIMER1_OVF_vect)
{
	TimerOverflow++;	/* Increment Timer Overflow count */
}

ISR(INT1_vect) //STEP2
{
   mode=1;
         _delay_ms(1500);
         GIFR=0xFF;
}

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
	
	char string[10];
	long count;
	int on=0;
	// double distance;

	
    int period,i;
	DDRD = 0x0f;		/* Make PORTD lower pins as output */
	period = 10;
	DDRA = 0x03;
	DDRC = 0xff;
	
	LCD_Init();
	LCD_String_xy(1, 0, "  Ultrasonic");
	
	sei();			/* Enable global interrupt */
	TIMSK = (1 << TOIE1);	/* Enable Timer1 overflow interrupts */
	TCCR1A = 0;		/* Set all bit to zero Normal operation */

    

	while(1)
	{
		/* Give 10us trigger pulse on trig. pin to HC-SR04 */
		PORTA |= (1 << Trigger_pin);
		_delay_us(10);
		PORTA &= (~(1 << Trigger_pin));
		
		TCNT1 = 0;	/* Clear Timer counter */
		TCCR1B = 0x41;	/* Capture on rising edge, No prescaler*/
		TIFR = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
		TIFR = 1<<TOV1;	/* Clear Timer Overflow flag */

		/*Calculate width of Echo by Input Capture (ICP) */
		
		while ((TIFR & (1 << ICF1)) == 0);/* Wait for rising edge */
		TCNT1 = 0;	/* Clear Timer counter */
		TCCR1B = 0x01;	/* Capture on falling edge, No prescaler */
		TIFR = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
		TIFR = 1<<TOV1;	/* Clear Timer Overflow flag */
		TimerOverflow = 0;/* Clear Timer overflow count */

		while ((TIFR & (1 << ICF1)) == 0);/* Wait for falling edge */
		count = ICR1 + (65535 * TimerOverflow);	/* Take count */
		/* 8MHz Timer freq, sound speed =343 m/s */
		distance = (double)count / 466.47 *8;

        double d=distance;
		dtostrf(distance, 2, 2, string);/* distance to string */
		strcat(string, " cm   ");	/* Concat unit i.e.cm */
		LCD_String_xy(2, 0, "Dist = ");
		LCD_String_xy(2, 7, string);	/* Print distance */
		 

        if (PINA & 0b00000100)
        {
            if(mode==0)
            {
                mode=1;
            }
            _delay_ms(50);
        }

		if(on==0 && distance<=15.0 && distance<1000&&mode==0)
		{
			on=1;
			for( i = 0; i<120; i++){
					PORTD = 0x09;
					_delay_ms(period);
					PORTD = 0x01;
					_delay_ms(period);
					PORTD = 0x03;
					_delay_ms(period);
					PORTD = 0x02;
					_delay_ms(period);
					PORTD = 0x06;
					_delay_ms(period);
					PORTD = 0x04;
					_delay_ms(period);
					PORTD = 0x0c;
					_delay_ms(period);
					PORTD = 0x08;
					_delay_ms(period);
			}
			EEPROM_write(0,'o');
			EEPROM_write(1,'n');
			EEPROM_write(2,' ');
			_delay_ms(1000);
		}
		else if(on==1 && distance>15.0&&distance<1000&&mode==0)
		{
			on=0;
			for( i = 0; i<140; i++){
						PORTD = 0x08;

						_delay_ms(period);
						PORTD = 0x0c;
						_delay_ms(period);
						PORTD = 0x04;
						_delay_ms(period);
						PORTD = 0x06;
						_delay_ms(period);
						PORTD = 0x02;
						_delay_ms(period);
						PORTD = 0x03;
						_delay_ms(period);
						PORTD = 0x01;
						_delay_ms(period);
						PORTD = 0x09;
					}
			
			EEPROM_write(0,'o');
			EEPROM_write(1,'f');
			EEPROM_write(2,'f');
			_delay_ms(1000);
		}
		else if(on==1 && distance<23.0 && distance<1000 && mode==1){
			on=0;
			for( i = 0; i<140; i++){
						PORTD = 0x08;
						_delay_ms(period);
						PORTD = 0x0c;
						_delay_ms(period);
						PORTD = 0x04;
						_delay_ms(period);
						PORTD = 0x06;
						_delay_ms(period);
						PORTD = 0x02;
						_delay_ms(period);
						PORTD = 0x03;
						_delay_ms(period);
						PORTD = 0x01;
						_delay_ms(period);
						PORTD = 0x09;
					}
					EEPROM_write(0,'o');
			EEPROM_write(1,'f');
			EEPROM_write(2,'f');
			_delay_ms(1000);
			mode=0;
			
		}
		else if(on==0 && distance<1000 && mode==1)
		{
			on=1;
			for( i = 0; i<120; i++){
					PORTD = 0x09;
					_delay_ms(period);
					PORTD = 0x01;
					_delay_ms(period);
					PORTD = 0x03;
					_delay_ms(period);
					PORTD = 0x02;
					_delay_ms(period);
					PORTD = 0x06;
					_delay_ms(period);
					PORTD = 0x04;
					_delay_ms(period);
					PORTD = 0x0c;
					_delay_ms(period);
					PORTD = 0x08;
					_delay_ms(period);
			}
			EEPROM_write(0,'o');
			EEPROM_write(1,'n');
			EEPROM_write(2,' ');
			_delay_ms(1000);
		}
		else{

		}

		_delay_ms(200);
	}
}
