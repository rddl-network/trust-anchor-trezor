#include <iostream>
#include <vector>
#include <sstream>

#include "bip3x_wrapper.h"
#include "utils.h"

#include <SLIPEncodedSerial.h>

HardwareSerial    SerialESP(1); // use UART1
SLIPEncodedSerial SLIPSerial(SerialESP);


void setup()
{
  Serial.begin(115200);
  SerialESP.begin(115200, SERIAL_8N1, 19, 18); // 2nd UART for XIAO ESP32-C3

  delay(20);
}

void loop(){
  OSCMessage msg;
  int i = 0;
  char c;
  int size;

  while(!SLIPSerial.endofPacket()){
    if( (size = SLIPSerial.available()) > 0)
    {
      while(size--)
        msg.fill(SLIPSerial.read());
    }
  }

  if (!msg.hasError()) 
  {
    msg.route("/IHW/bip39Mnemonic", routeBip39Mnemonic);
    msg.route("/IHW/bip39MnemonicFromBytes", routeBip39Mnemonic);
    msg.route("/IHW/bip39MnemonicToSeed", routeBip39MnemonicToSeed);
    msg.route("/IHW/bip32_key_from_seed", routeBip32KeyFromSeed);
    msg.route("/IHW/valiseSeedSet", routeValiseSeedSet);
    msg.route("/IHW/valiseSeedGet", routeValiseSeedGet);
    msg.route("/IHW/valiseSignDigest", routeSignDigest);
    msg.route("/IHW/valiseVerifySign", routeVerifySign);
  }
}