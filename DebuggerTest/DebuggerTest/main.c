/*
 * DebuggerTest.c
 *
 * Created: 03-06-2022 08:27:45
 * Author : bla
 */ 

#include <avr/io.h>

void bitFlip(uint8_t *port, uint8_t bit){
	*port ^= (1 << bit);	
}


int main(void) //Using JTAG
{
	uint8_t *ddr = 0x24;
	*ddr |= (1 << 2);
	*ddr |= (1 << 1);
	*ddr |= (1 << 0);
	
	uint8_t pos = 0;
	
	uint8_t *port = 0x25;	
	*port |= (1 << 2);
	*port |= (1 << 1);
	*port |= (1 << 0);
    while (1) // Reason for no delay is that it is only meant to be run through debugging using JTAG.
    {
		bitFlip(port,pos);
		pos = (++pos) % 3; 
    }
}

