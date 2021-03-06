#pragma once

typedef void (*Function_Pointer)(char *);

#define StringCharMaxAmount 256

typedef enum
{
	ADDRESS_STATE_RECEIVING_ADDRESS,
	ADDRESS_STATE_RECEIVING_BIT_POSITION,
	ADDRESS_STATE_RECEIVING_BIT_VALUE,
	ADDRESS_STATE_UNKNOWN,
} AddressStates;

typedef enum
{
	OUTER_STATE_IDLE,
	OUTER_STATE_ADDRESS,
	OUTER_STATE_TIMER,
	OUTER_STATE_UNKNOWN,
} OuterStates;

typedef enum {
	TIMER_STATE_UNKNOWN,
} TimerStates;

typedef struct {
	AddressStates AddressState;
	Function_Pointer ThisFunctionPointer;
	PGM_P ptrToFlahString;
} AddressMachineStruct;

typedef struct {
	TimerStates TimerState;
	Function_Pointer ThisFunctionPointer;
	PGM_P ptrToFlashString;
} TimerMachineStruct;

typedef struct {
	OuterStates OuterState;
	Function_Pointer ThisFunctionPointer;
	PGM_P ptrToFlashString;
} OuterMachineStruct;

#define AddressStringCharMax 4

typedef struct
{
	uint16_t Address;
	uint8_t Bit_Position;
	uint8_t Bit_State;
	char Address_String[AddressStringCharMax + 1];
	uint8_t Address_StringPointer;
} AddressStruct;


