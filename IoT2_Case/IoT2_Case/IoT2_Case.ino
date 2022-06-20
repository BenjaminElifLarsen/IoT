
#include "FlashProm.h"
#define USE_SERIAL Serial

#include "ProjectDefinitions.h"

static volatile AddressStates AddressState = ADDRESS_STATE_RECEIVING_ADDRESS;
static volatile AddressStruct WorkingAddresStruct;
AddressMachineStruct* AddressStructInRAM_Pointer;

static volatile OuterStates OuterState = OUTER_STATE_IDLE;
OuterMachineStruct* OuterStructInRAM_Pointer;

char* StringPointer;

int my_putc(char c, FILE* t) 
{
	USE_SERIAL.write(c);
}

void RestartStates(void)
{
	OuterState = OUTER_STATE_IDLE;
	AddressState = ADDRESS_STATE_RECEIVING_ADDRESS;
	WorkingAddresStruct.Address_StringPointer = 0;
}

void ChangeAddrssState(void) 
{
	uint8_t stateInt = (int)AddressState;
	stateInt = (++stateInt) % ADDRESS_STATE_UNKNOWN;
	AddressState = (AddressStates)stateInt;
	WorkingAddresStruct.Address_StringPointer = 0;
}



void setup() {
}

void loop() {
}
