// FlashProm.h

#ifndef _FLASHPROM_h
#define _FLASHPROM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

extern const char AddressStateReceivingAddresss[] PROGMEM;
extern const char AddressStateReceivingBitPosition[] PROGMEM;
extern const char AddressStateReceivingBitValue[] PROGMEM;
extern const char AddressStateUnknown[] PROGMEM;

extern const char OuterStateIdle[] PROGMEM;
extern const char OuterStateAddrss[] PROGMEM;
extern const char OuterStateTimer[] PROGMEM;
extern const char OuterStateUnknown[] PROGMEM;

extern uint16_t strlen_FlashProm(const char* FlashProm_Address);
extern uint16_t strlen_EE_Prom(const char* FlashProm_Address);
extern void strcpy_FlahProm(char* RAM_Malloc_Pointer, const char* FlashProm_Address);
extern void strcpy_EE_Prom(char* RAM_Malloc_Pointer, const char FlashProm_Address);
extern void memcpy_FlashProm(char* RAM_Malloc_Pointer, const char* FlahProm_Address, uint16_t NumberOfBytes);




#endif

