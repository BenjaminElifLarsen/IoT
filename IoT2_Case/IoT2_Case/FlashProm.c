// 
// 
// 

#include "FlashProm.h"
#include "ProjectDefinitions.h"

const char AddressStateReceivingAddresss[] PROGMEM = "\nEnter Address in Hex (max 4 sign): 0x";
const char AddressStateReceivingBitPosition[] PROGMEM = "\nEnter bit position (0 - 7): ";
const char AddressStateReceivingBitValue[] PROGMEM = "\nEnter bit value (0 - 1):";
const char AddressStateUnknown[] PROGMEM = "\nUnknown address state";

const char OuterStateIdle[] PROGMEM = "\nCurrent outer state: Idle";
const char OuterStateAddrss[] PROGMEM = "\nCurrent outer state: Address";
const char OuterStateTimer[] PROGMEM = "\nCurrent outer state: Timer";
const char OuterStateUnknown[] PROGMEM = "\nCurrent outer state: Unknown";

const char TimerStateUnknown[] PROGMEM = "\nUnknown timer state";

uint16_t strlen_FlashProm(const char* FlashProm_Address) 
{
	uint16_t flahPromStringAddress;
	uint16_t numberOfBytes = 0;
	uint8_t flashPromValueInByte;

	flahPromStringAddress = (uint16_t)FlashProm_Address;

	while ( ((flashPromValueInByte = pgm_read_byte(flahPromStringAddress)) != 0) && (numberOfBytes < StringCharMaxAmount) )
	{
		numberOfBytes++;
		flahPromStringAddress++;
	}

	return numberOfBytes + 1;
}

uint16_t strlen_EE_Prom(const char* FlashProm_Address) 
{
	uint16_t eePromStringAddress;
	uint16_t numberOfBytes = 0;
	uint8_t eePromValueInByte;

	eePromStringAddress = pgm_read_word((uint16_t)FlashProm_Address);

	while (((eePromValueInByte = eeprom_read_byte((const uint8_t*)eePromStringAddress)) != 0) && (numberOfBytes < StringCharMaxAmount))
	{
		numberOfBytes++;
		eePromStringAddress++;
	}

	return numberOfBytes + 1;
}

void strcpy_FlahProm(char* RAM_Malloc_Pointer, const char* FlashProm_Address) 
{
	strcpy_P(RAM_Malloc_Pointer, (PGM_P)pgm_read_word(FlashProm_Address));
}

void strcpy_EE_Prom(char* RAM_Malloc_Pointer, const char FlashProm_Address) 
{
	uint16_t eePromStringAddress;

	eePromStringAddress = pgm_read_word((uint16_t)FlashProm_Address);
	
	EEprom_Read_String(RAM_Malloc_Pointer, eePromStringAddress);
}

void memcpy_FlashProm(char* RAM_Malloc_Pointer, const char* FlahProm_Address, uint16_t NumberOfBytes) 
{
	memccpy_P(RAM_Malloc_Pointer, FlahProm_Address, NumberOfBytes);
}

