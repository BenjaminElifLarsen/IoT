#include <XBee.h>


XBee xbee = XBee();

ZBRxResponse rx = ZBRxResponse();


long msb = 1286656;//0x0013a200;
long lsb = 1090976252;//0x4106f9fc;
bool high = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  xbee.setSerial(Serial);
  Serial.println(msb, HEX);
  Serial.println(lsb, HEX);
}

void loop() {
  uint8_t payloadHigh[] = {(uint8_t)'1'};
  uint8_t payloadLow[] = {(uint8_t)'0'};

  XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x4106f9fc);

  if (1 == high) {
    ZBTxRequest zbTx = ZBTxRequest(addr64, payloadHigh, sizeof(payloadHigh));
    xbee.send(zbTx);
    if (xbee.readPacket(2500)) {

    } else {
      Serial.println(xbee.getResponse().getErrorCode());
    }
    high = 0;
  }
  else {
    ZBTxRequest zbTx = ZBTxRequest(addr64, payloadLow, sizeof(payloadLow));
    xbee.send(zbTx);
    high = 1;
    if (xbee.readPacket(2500)) {

    } else {
      Serial.println(xbee.getResponse().getErrorCode());
    }
  }

  Serial.println("\n");
  Serial.println(high);
  delay(1000);
}
