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
	
	// With the code here the bit 4, on port B, is set. 
	PORTB |= (1 << PB4);
	
	PORTB = 0x11; 
	//PORTB |= (1 << PB3);
	//PORTB |= (1 << PB5);
	// With this code bit 3 and bit 5, on port b, are set. 
	PORTB |= (1 << PB3) | (1 << PB5);
	
    // This code rests bit 0 to 0. 
    PORTB &= ~(1 << PB0);

	// This code rests bit 3 and bit 5 to 0.
	PORTB &= ~((1 << PB3) | (1 << PB5));
	
    //This code toggles bit 2 in port B. 
	PORTB ^= (1 << PB2);
 
	PORTB ^= (1 << PB6) | (1 << PB7);
	// This will do nothing since the bits are already 1.
	PORTB |= (1 << PB6) | (1 << PB7);
	
	//Ensures it is 0x14
	PORTB ^= (1 << PB6) | (1 << PB7);
	//Sets it to 0x15 since the logical OR will return 1 since either side do not give 0. This would not be a common thing to do, would most like by an error after teacher.
	PORTB |= (1 << PB6) || (1 << PB7);


    //while (1) 
    //{
						//
    //}
}

