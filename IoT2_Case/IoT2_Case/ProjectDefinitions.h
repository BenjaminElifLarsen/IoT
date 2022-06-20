#pragma once

typedef void (*Function_Pointer)(char *);

typedef enum
{
	ADDRESS_STATE_RECEIVING_ADDRESS,
	ADDRESS_STATE_BIT_POSITION,
	ADDRESS_STATE_BIT_VALUE,
	ADDRESS_STATE_UNKNOWN,
} AddressStates;

typedef enum
{
	OUTER_STATE_ADDRESS,
	OUTER_STATE_TIMER,
	OUTER_STATE_UNKNOWN,
} OuterStates;

typedef enum {

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

typedef struct
{
	uint16_t Address;
	uint8_t Bit_Position;
	uint8_t Bit_State;
} AddressStruct;


