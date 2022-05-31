
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32 
#define OLED_RESET     4 
#define SCREEN_ADDRESS 0x3C 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

extern "C"{
  void RS232Init();
}

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
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  RS232Init();
  
  Serial.begin(9600);
  currentState = Receive_Address;
}

void WriteDisplay(String message, int y) {
  display.setCursor(0, y);
  display.println(message);
  Serial.println(message);
  display.display();
}

const char* getStateName(enum states state) {
  switch (state) {
    case Receive_Address: return "Receive_Address";
    case Receive_Bit_Position_In_Address: return "Receive_Bit_Position_In_Address";
    case Receive_Bit_Position_Value: return "Receive_Bit_Position_Value";
  }
}

unsigned long Read(int readLength) {
  while (Serial.available() < readLength + 2) {} 
  /*
   * If switching from Serial to UART considerations:
   * Perhaps have no reason to have something like the line of code about,
   * instead the program could just read from the UART and discard all data before '<'. 
   * When its encounter the '<' it starts storing the values until it reads the '>'.
   * Then read all values after until there is none left. 
   * When this is done the program can process the array.
   * Got problems with getting FDEV_SETUP_STREAM to work 'sorry, unimplemented: non-trivial designated initializers not supported'
   */
  
  char bufferArray[readLength]; 

  bool hasStarted = false;
  bool hasEnded = false;
  bool hasError = false;
  int pos = 0;
  for (int i = 0; i < readLength + 2; i++) { 
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
    else if(1 == i && '0' == value){
      bufferArray[pos] = value;
      pos++;
    }
    else if(2 == i && (value == 'x' || value == 'X') ){
      bufferArray[pos] = value;
      pos++;
    }
    else if (i > 2 && isxdigit(value) != 0 && hasStarted && !hasEnded && !hasError) {
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
  
  bool validValue = false;
  if(value <= 255){
    validValue = true;
  }
  if (true == validValue) {
    address = value;
    currentState = Receive_Bit_Position_In_Address;
    display.clearDisplay();
    display.display();
    WriteDisplay("A:" + String(address, HEX), 0);
    Serial.println(getStateName(currentState));
  }
}

void ReceiveBitPosition() {
  unsigned long value = Read(3);

  bool validValue = false;
  if (value <= 7) {
    validValue = true;
  }
  if (true == validValue) {
    bitPosition = value;
    currentState = Receive_Bit_Position_Value;
    WriteDisplay("P:" + String(bitPosition, HEX), 8);
    Serial.println(getStateName(currentState));

  }

}

void ReceiveBitValue() {
  unsigned long value = Read(3);
  //do something

  bool validValue = false;
  if (0 == value || 1 == value) {
    validValue = true;
    WriteDisplay("B:" + String(value, HEX), 16);
  }
  if (true == validValue) {

    uint8_t *port = reinterpret_cast<uint8_t*>(address);
    uint8_t *ddr = reinterpret_cast<uint8_t*>(address); // After datasheet, ddr address is one below the port address.
    ddr--;

    Serial.println(*ddr, BIN);
    Serial.println(*port, BIN);
    if (1 == value)
    {
      *ddr |= (1 << bitPosition);
      *port |= (1 << bitPosition);
    }
    else {
      *port = *port & ~(1 << bitPosition);
      *ddr = *ddr & ~(1 << bitPosition);
    }

    Serial.println(*ddr, BIN);
    Serial.println((uint8_t)*port, BIN);

    currentState = Receive_Address;
    Serial.println(getStateName(currentState));
    WriteDisplay("BIN:" + String((uint8_t)*port, BIN), 24);
  }
}

void loop() {
  switch(currentState){
    case Receive_Address:
      ReceiveAddress();
      break;
    case Receive_Bit_Position_In_Address:
      ReceiveBitPosition();
      break;
    case Receive_Bit_Position_Value: 
      ReceiveBitValue(); 
      break;
  }
}
