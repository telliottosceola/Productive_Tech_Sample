#include <Arduino.h>
#include <MCP23008.h>
#include <HardwareSerial.h>

MCP23008 relay;
HardwareSerial serial1(2);

void inputChangeHandler(uint8_t input, uint8_t newState);

unsigned long interval = 2000;
unsigned long lastSwitch = 0;
int bankState;

void setup() {
  // put your setup code here, to run once:
  relay.setRelays(4);
  relay.setInputs(255);
  relay.registerInputChangeCallback(inputChangeHandler);
  relay.init(10);

  Serial.begin(115200);
  serial1.begin(115200);
}

void loop() {
  if(serial1.available()){
    delay(50);
    char buffer[serial1.available()];
    serial1.read(buffer, sizeof(buffer));
    Serial.printf("Received: %s\n", buffer);
  }
  //Update state of relays in loop
  if(millis() > lastSwitch+interval){
    lastSwitch = millis();
    if(bankState < 16){
      bankState++;
      relay.setBankStatus(bankState);
    }else{
      bankState = 0;
      relay.setBankStatus(0);
    }
  }
  //loop the board to check inputs
  relay.loop();
}

void inputChangeHandler(uint8_t input, uint8_t newState){
  Serial.printf("Input %i new state: %i\n",input,newState);
}
