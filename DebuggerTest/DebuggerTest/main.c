/*
 * DebuggerTest.c
 *
 * Created: 03-06-2022 08:27:45
 * Author : bla
 */ 

#include <avr/io.h>

void bit0(uint8_t *port){
	*port ^= (1 << 0);	
}

void bit1(uint8_t *port){
	*port ^= (1 << 1);	
}

void bit2(uint8_t *port){
	*port ^= (1 << 2);	
}

int main(void) //Using JTAG
{
    /* Replace with your application code */
	
	uint8_t *ddr = 0x24;
	*ddr |= (1 << 2);
	*ddr |= (1 << 1);
	*ddr |= (1 << 0);
	
	uint8_t pos = 0;
	
	uint8_t *port = 0x25;
	
	*port |= (1 << 2);
	*port |= (1 << 1);
	*port |= (1 << 0);
    while (1) 
    {
		if(2 == pos){
			bit2(port);
		}
		else if(1 == pos){
			bit1(port);
		}
		else if(0 == pos){
			bit0(port);
		}
		pos = (++pos) % 3; 
    }
}

