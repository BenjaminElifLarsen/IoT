
#include "FlashProm.h"
#define USE_SERIAL Serial3

#include "ProjectDefinitions.h"
#include "FlashProm.h"

#include "SoftwareSerial.h"
#include "avr8-stub.h"
#include "app_api.h"

static volatile AddressStates AddressState = ADDRESS_STATE_RECEIVING_ADDRESS;
static volatile AddressStruct WorkingAddresStruct;
AddressMachineStruct* AddressStructInRAM_Pointer;

static volatile OuterStates OuterState = OUTER_STATE_IDLE;
OuterMachineStruct* OuterStructInRAM_Pointer;

char* StringPointer;
uint16_t NumberOfCharsInString;

int IncomingByte = 0;
char IncomingChar = '0';
static volatile bool PrintOut = true;
static volatile bool EndOfStateMachine = false;

int my_putc(char c, FILE* t) 
{
	USE_SERIAL.write(c);
}

void RestartStates(void)
{
	OuterState = OUTER_STATE_IDLE;
	AddressState = ADDRESS_STATE_RECEIVING_ADDRESS;
	WorkingAddresStruct.Address_StringPointer = 0;
	PrintOut = true;
}

void ChangeAddrssState(void) 
{
	uint8_t stateInt = (int)AddressState;
	stateInt = (++stateInt) % ADDRESS_STATE_UNKNOWN;
	AddressState = (AddressStates)stateInt;
	WorkingAddresStruct.Address_StringPointer = 0;
	PrintOut = true;
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

	if ((*ch < '0') || (*ch > '1')) 
	{
		RestartStates();
	}

	WorkingAddresStruct.Bit_State = *ch - '0';
	address_Pointer = (uint8_t*)(uint16_t)strtoul((const char*)WorkingAddresStruct.Address_String, NULL, 16);

	if (0 == WorkingAddresStruct.Bit_State)
	{
		uint8_t* ddr = address_Pointer;
		ddr--;
		*ddr &= ~(1 << WorkingAddresStruct.Bit_Position);
		*address_Pointer &= ~(1 << WorkingAddresStruct.Bit_Position);
	}
	else 
	{
		uint8_t* ddr = address_Pointer;
		ddr--;
		*ddr |= 1 << WorkingAddresStruct.Bit_Position;
		*address_Pointer |= 1 << WorkingAddresStruct.Bit_Position;
	}

	EndOfStateMachine = true;
	RestartStates();

}

const AddressMachineStruct PROGMEM AddressMachineStructArray[] =
{
	{ADDRESS_STATE_RECEIVING_ADDRESS, ReceivedCharAsAddressChar, AddressStateReceivingAddresss},
	{ADDRESS_STATE_RECEIVING_BIT_POSITION, ReceivedCharAsPositionChar, AddressStateReceivingBitPosition},
	{ADDRESS_STATE_RECEIVING_BIT_VALUE, ReceivedCharAsValueChar, AddressStateReceivingBitValue},
};

AddressMachineStruct* AddressStateAllocateMemoryInRamAndGetCopyFromFlashProm()
{
	AddressMachineStruct* MachineStructInRAM_Pointer = (AddressMachineStruct*)malloc(sizeof(AddressMachineStruct));
	memcpy_FlashProm((char*)MachineStructInRAM_Pointer, (const char*)&(AddressMachineStructArray[(uint8_t)AddressState]), sizeof(AddressMachineStruct));
}

void UartRecivedCharCovertArcoordingToState(char inputChr)
{
	AddressMachineStruct* addresMachineStructInRAM_Pointer;
	addresMachineStructInRAM_Pointer = AddressStateAllocateMemoryInRamAndGetCopyFromFlashProm();
	addresMachineStructInRAM_Pointer->ThisFunctionPointer(&inputChr);
	free(addresMachineStructInRAM_Pointer);
}

void AddressStateRun()
{
	if (USE_SERIAL.available() > 0)
	{
		IncomingByte = USE_SERIAL.read();
		IncomingChar = static_cast<char>(IncomingByte);
		UartRecivedCharCovertArcoordingToState(IncomingChar);
	}

	if (true == EndOfStateMachine)
	{
		EndOfStateMachine = false;
		return;
	}
	
	if (true == PrintOut)
	{
		PrintOut = false;
		AddressStructInRAM_Pointer = AddressStateAllocateMemoryInRamAndGetCopyFromFlashProm();
		NumberOfCharsInString = strlen_FlashProm((const char*)AddressStructInRAM_Pointer->ptrToFlahString);
		StringPointer = (char*)malloc(NumberOfCharsInString);
		memcpy_FlashProm((char*)StringPointer, (const char*)(AddressStructInRAM_Pointer->ptrToFlahString), NumberOfCharsInString);
		printf(StringPointer);
	}
}


void TimerStateRun() {

	printf("\nWIP");
	OuterState = OUTER_STATE_IDLE;
	if (true == EndOfStateMachine) {
		EndOfStateMachine = false;
		return; 
	}

}

void OuterStateHandling() {
	printf("\nEnter Number\n1 For Address\n2 For Timer\nValue:");
	while(!USE_SERIAL.available()){}
	int data = USE_SERIAL.read();
	while (USE_SERIAL.available()) { USE_SERIAL.read(); }
	if (data == '1') {
		OuterState = OUTER_STATE_ADDRESS;
		return;
	}
	else if (data == '2') {
		OuterState = OUTER_STATE_TIMER;
		return;
	}
	printf("\nUnknown Value");
	OuterState = OUTER_STATE_IDLE;
}


void setup() {
	USE_SERIAL.begin(9600);
	debug_init();
	delay(3000);

	fdevopen(&my_putc, 0);

	OuterState = OUTER_STATE_IDLE;
	WorkingAddresStruct.Address_StringPointer = 0;
	WorkingAddresStruct.Address_String[StringCharMaxAmount] = "\0";
	printf("\nStartingUp\n");
}

void loop() {
	switch (OuterState)
	{
	case OUTER_STATE_IDLE:
		OuterStateHandling();
		break;

	case OUTER_STATE_ADDRESS: 
		AddressStateRun();
		break;

	case OUTER_STATE_TIMER:
		TimerStateRun();
		break;
	}
}
