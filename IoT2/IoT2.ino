enum states {
  Receive_Address,
  Receive_Bit_Position_In_Address,
  Receive_Bit_Position_Value
};

states currentState;

const char START_CHAR = '<';
const char END_CHAR = '>';

void setup() {
  //to do, set up screen
  Serial.begin(9600);
  currentState = Receive_Address;
}

void Read(){
  //test code
  char bufferArray[] = {'0','x','6','4'};
  unsigned long value; 
  value = strtoul(bufferArray, NULL,16);
  Serial.print(value);
}

void ReceiveAddress(){
    //do something
    bool validValue = false;
    if(true == validValue){
      currentState = Receive_Bit_Position_In_Address;
      
    }  
}

void ReceiveBitPosition(){
      //do something
    bool validValue = false;
    if(true == validValue){
      currentState = Receive_Bit_Position_Value;
      
    }
}

void ReceiveBitValue(){
      //do something
    bool validValue = false;
    if(true == validValue){
      currentState = Receive_Address; 
      
    }
}

void loop() {
  Read();
  if(Receive_Address == currentState){ 
    ReceiveAddress();
  }
  else if(Receive_Bit_Position_In_Address == currentState){
    ReceiveBitPosition();
  }
  else if(Receive_Bit_Position_Value == currentState){
    ReceiveBitValue();
  }
}
