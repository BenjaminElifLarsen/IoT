/*
 * DebuggerTest.c
 *
 * Created: 03-06-2022 08:27:45
 * Author : bla
 */ 

#include <avr/io.h>

void bitFlip(uint8_t *port, uint8_t bit){
	uint8_t *ddr = port;	
	ddr--;
	*ddr ^= (1 << bit);
	*port ^= (1 << bit);	
}


int main(void) //Using JTAG
{	
	uint8_t *port = 0x25;	
	uint8_t pos = 0;	
    while (1) // Reason for no delay is that it is only meant to be run through debugging using JTAG with a breakpoint in the loop.
    {
		bitFlip(port,pos);
		pos = (++pos) % 3; 
    }
}

