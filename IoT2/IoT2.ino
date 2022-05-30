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

unsigned long Read(int readLength){
  while(Serial.available() < readLength+2){}

  char bufferArray[readLength]; //= {'0','x','7','B','b'};

  bool hasStarted = false;
  bool hasEnded = false;
  bool hasError = false;
  int pos = 0;
  int dataSize = Serial.available();
  
  for(int i = 0; i < readLength+2; i++){
    char value = Serial.read();
    if(0 == i){
      if(START_CHAR == value){
        hasStarted = true;
      }
      else{
        hasError = true;
        break;
      }
    }
    else if(END_CHAR == value){
      hasEnded = true;
    }
    else if(hasStarted && !hasEnded && !hasError){
      bufferArray[pos] = value;
      pos++;
    }   
  }
  
  do{
    (void)Serial.read();
  }while(Serial.available());
  
  if(!hasError && hasStarted && hasEnded){
    unsigned long readValue; 
    readValue = strtoul(bufferArray, NULL,16);
    Serial.println(readValue);
    return readValue;
  }
  return 4294967295;
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
  Read(3);
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
