/*
 * FirstProject.c
 *
 * Created: 17-05-2022 11:04:55
 * Author : bla
 */ 

#include <avr/io.h>


int main(void)
{
	DDRB = 0xFF; //used for setting the direction (INPUT or OUTPUT) of gpio pins, DATA DIRECT REGISTER
	PORTB |= 0x26; //used for enabling/ disabling the internal pull-up resistor in case of input pin and it is also used for setting the pin as HIGH/ LOW in case of output pin.
	
    while (1) 
    {
				
    }
}

