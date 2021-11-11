#ifndef MCP23008_H
#define MCP23008_H
#include <Arduino.h>
#include <Wire.h>
class MCP23008{
public:
  //Constructor
  MCP23008();
  virtual ~MCP23008();
  //Set Address.  Indicate status of jumpers on board.  Send 0 for not installed, send 1 for installed
  void setAddress(int a0, int a1, int a2);
  void setAddress(int a);
  //Set number or outputs, for relay boards they will start at 0, if you need outputs at specific locations
  //use the setAsOutput method for each, or call setOutputs with a uint8_t map where each bit is either 1 (an output) or 2 (an input)
  void setRelays(int num);
  void setOutputs(int map);
  void setOutput(int num);
  void relayTimerMillis(int relay, unsigned long milliSeconds);

  //The map is a single uint8_ts with the bits set for inputs that should be pulled up e.g. 00000011 (or 3) will pull inputs 0 and 1 high, all others will be floating
  void setInputs(int map);

  void init(int debounce = 5);
  //Turn on Relay
  void turnOnRelay(int relay, bool desired=false);
  //Turn off Relay
  void turnOffRelay(int relay, bool desired=false);
  //Toggle Relay
  void toggleRelay(int relay);
  //momentary relay
  // void momentaryRelay(int relay);
  // void momentaryRelay(int relay, int duration);
  // void momentaryOff();

  //Turn On all Relays
  void turnOnAllRelays(bool desired=false);
  //Turn Off All Relays
  void turnOffAllRelays(bool desired=false);

  uint8_t bankStatus;
  //Set status of all relays in bank
  void setBankStatus(int status);
  //Read status of relay. Valid return 0 for off 1 for on.  256 returned if there is an error
  int readRelayStatus(int relay);
  //Read status of all relays in bank.  0-255 valid.  256 returned on error
  int readRelayBankStatus();
  //Read status of input
  int readInputStatus(int input);
  //Read status of all inputs
  int readAllInputs();
  int relayTalk(String command);
  //User Accessible Variables
  //Whether or not the controller is ready to accept commands
  bool initialized;
  bool publishInputEvents;
  int sendCommand(int reg, int cmd);
  int sendCommand(int reg);

  void registerInputChangeCallback(void(*inputChangeCallback)(uint8_t channel, uint8_t newState));
  void loop(bool fireCallback = true);
private:
  uint8_t relayDesired = 0;
  uint8_t previousDesired = 0;

  bool firstInput = true;

  uint8_t bitop(uint8_t b1, uint8_t b2, int op);
  void relayOp(int relay, int op, bool desired=false);
  //internal use method for refreshing bank status variables
  void readStatus();
  //Status of relays in bank 1

  uint8_t inputStatus;
  uint8_t outputMap = 255;
  uint8_t inputMap = 0;

  bool iosetCustom = false;
  int relayCount = 0;

  int _momentaryRelay;

  int address = 0x20;

  int retrys = 0;

  void (*_inputChangeCallback)(uint8_t channel, uint8_t newState);

  bool tripped[8];

  int debugTrips[8];

  int minTrips;
  unsigned long relayTimerStart[8];
  unsigned long relayTimerDuration[8];
};
#endif
