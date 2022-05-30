enum states {
  Receive_Address,
  Receive_Bit_Position_In_Address,
  Receive_Bit_Position_Value
};

states currentState;

const char START_CHAR = '<';
const char END_CHAR = '>';

uint8_t address;
uint8_t bitPosition;

void setup() {
  //to do, set up screen
  Serial.begin(9600);
  currentState = Receive_Address;
}

unsigned long Read(int readLength) {
  while (Serial.available() < readLength + 2) {}

  char bufferArray[readLength]; //= {'0','x','7','B','b'};

  bool hasStarted = false;
  bool hasEnded = false;
  bool hasError = false;
  int pos = 0;
  for (int i = 0; i < readLength + 2; i++) { //should check if the second value is 0 and third is x/X
    char value = Serial.read();
    if (0 == i) {
      if (START_CHAR == value) {
        hasStarted = true;
      }
      else {
        hasError = true;
        break;
      }
    }
    else if (END_CHAR == value) {
      hasEnded = true;
    }
    else if ((value == 'x' || value == 'X' || isHexadecimalDigit(value)) && hasStarted && !hasEnded && !hasError) {
      bufferArray[pos] = value;
      pos++;
    }
    else {
      hasError = true;
      Serial.println(value);
      break;
    }
  }

  do {
    (void)Serial.read();
  } while (Serial.available());

  //Serial.println(bufferArray);
  if (!hasError && hasStarted && hasEnded) {
    unsigned long readValue;
    readValue = strtoul(bufferArray, NULL, 16);
    //Serial.println(readValue);
    return readValue;
  }
  return 4294967295;
}

void ReceiveAddress() {
  unsigned long value = Read(4);
  //do something

  bool validValue = true;
  if (true == validValue) {
    address = value;
    currentState = Receive_Bit_Position_In_Address;
    Serial.println(currentState);
  }
}

void ReceiveBitPosition() {
  unsigned long value = Read(4);
  //do something

  bool validValue = true;
  if (true == validValue) {
    bitPosition = value;
    currentState = Receive_Bit_Position_Value;
    Serial.println(currentState);

  }
}

void ReceiveBitValue() {
  unsigned long value = Read(3);
  //do something

  bool validValue = false;
  if (0 == value || 1 == value) {
    validValue = true;
  }
  if (true == validValue) {

    uint8_t *port = &address;
    uint8_t *ddr = &address; // After datasheet, ddr address is one below the port address.
    ddr--;

    Serial.println((uint8_t)*ddr,HEX);
    Serial.println((uint8_t)*port,HEX);
    Serial.println((uint8_t)&ddr,HEX);
    Serial.println((uint8_t)&port,HEX);
    if (1 == value)
    {
      *ddr |= (1 << bitPosition);
      *port |= (1 << bitPosition);
    }
    else {
      *port = *port & ~(1 << bitPosition);
      *ddr = *ddr & ~(1 << bitPosition);
    }

    Serial.println((uint8_t)*ddr,HEX);
    Serial.println((uint8_t)*port,HEX);
    
    currentState = Receive_Address;
    Serial.println(currentState);
  }
}

void loop() {
  if (Receive_Address == currentState) {
    ReceiveAddress();
  }
  else if (Receive_Bit_Position_In_Address == currentState) {
    ReceiveBitPosition();
  }
  else if (Receive_Bit_Position_Value == currentState) {
    ReceiveBitValue();
  }
}
