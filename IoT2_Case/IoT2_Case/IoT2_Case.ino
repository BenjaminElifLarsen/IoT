
#include "FlashProm.h"
#define USE_SERIAL Serial3

#include "ProjectDefinitions.h"

static volatile AddressStates AddressState = ADDRESS_STATE_RECEIVING_ADDRESS;
static volatile AddressStruct WorkingAddresStruct;
AddressMachineStruct* AddressStructInRAM_Pointer;

static volatile OuterStates OuterState = OUTER_STATE_IDLE;
OuterMachineStruct* OuterStructInRAM_Pointer;

char* StringPointer;
uint16_t NumberOfCharsInString;

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

void ReceivedCharAsAddressChar(char* ch)
{
	if (!isxdigit(*ch)) {
		RestartStates();
	}

	WorkingAddresStruct.Address_String[WorkingAddresStruct.Address_StringPointer++] = *ch;
	
	if (AddressStringCharMax == WorkingAddresStruct.Address_StringPointer) {
		ChangeAddrssState();
	}
}

void ReceivedCharAsPositionChar(char* ch) 
{
	if (!((*ch >= '0') && (*ch <= '7')))
	{
		RestartStates();
	}

	WorkingAddresStruct.Bit_Position = *ch - '0';
	ChangeAddrssState();
}

void ReceivedCharAsValueChar(char* ch) {
	uint8_t* address_Pointer;

	if (!((*ch >= '0') && (*ch <= '1'))) 
	{
		RestartStates();
	}

	WorkingAddresStruct.Bit_State = *ch - '0';
	address_Pointer = (uint8_t*)(uint16_t)strtoul((const char*)WorkingAddresStruct.Address_String, NULL, 16);

	bitFlip(address_Pointer, WorkingAddresStruct.Bit_State);
	RestartStates();

}

void bitFlip(uint8_t* port, uint8_t bit) 
{
	uint8_t* ddr = port;
	ddr--;
	*ddr ^= (1 << bit);
	*port ^= (1 << bit);
}

const AddressMachineStruct PROGMEM AddressMachineStructArray[] =
{
	{ADDRESS_STATE_RECEIVING_ADDRESS, ReceivedCharAsAddressChar, AddressStateReceivingAddresss},
	{ADDRESS_STATE_RECEIVING_BIT_POSITION, ReceivedCharAsPositionChar, AddressStateReceivingBitPosition},
	{ADDRESS_STATE_RECEIVING_BIT_VALUE, ReceivedCharAsValueChar, AddressStateReceivingBitValue},
};

void AddressStateRun()
{
	//main code call, from the loop function, that is run when the outer state is address
}

AddressMachineStruct* AddressStateAllocateMemoryInRamAndGetCopyFromFlashProm()
{
	AddressMachineStruct* MachineStructInRAM_Pointer = (AddressMachineStruct*)malloc(sizeof(AddressMachineStruct));
	memcpy_FlashProm((char*)MachineStructInRAM_Pointer, (const char*)&(AddressMachineStructArray[(uint8_t)AddressState]), sizeof(AddressMachineStruct));
}

void setup() {
}

void loop() {
}
