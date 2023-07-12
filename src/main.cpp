#include <iostream>
#include <vector>
#include <sstream>

#include "OCC/bip32/occ_bip32.h"
#include "OCC/bip39/occ_bip39.h"
#include "OCC/valise/valise.h"

#ifdef DSE050
  #include "OCC/se050/se050.h"
  #include "se050_middleware.h"
#endif

#include <SLIPEncodedSerial.h>

HardwareSerial    SerialESP(1); // use UART1
SLIPEncodedSerial SLIPSerial(SerialESP);

void setup()
{
  Serial.begin(115200);
  SerialESP.setRxBufferSize(1024);
  SerialESP.setTxBufferSize(1024);
  SerialESP.begin(115200, SERIAL_8N1, 19, 18); 

#ifdef DSE050
  se050_obj.init_interface(5, 6);
#endif
  
  delay(20);
}

void loop(){
  OSCMessage msg;
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
    /* Bip39 functions */
    msg.route("/IHW/bip39Mnemonic", routeBip39Mnemonic);
    msg.route("/IHW/bip39MnemonicFromBytes", routeBip39Mnemonic);
    msg.route("/IHW/bip39MnemonicToSeed", routeBip39MnemonicToSeed);

    /* Bip32 functions */
    msg.route("/IHW/bip32_key_from_seed", routeBip32KeyFromSeed);

    /* Valise functions */
    msg.route("/IHW/valiseSeedSet", routeValiseSeedSet);
    msg.route("/IHW/valiseSeedGet", routeValiseSeedGet);
    msg.route("/IHW/valiseSignDigest", routeSignDigest);
    msg.route("/IHW/valiseVerifySign", routeVerifySign);

    /* SE050 functions */
#ifdef DSE050
    msg.route("/IHW/se050SetSeed", routeSE050EncryptData);
    msg.route("/IHW/se050GetSeed", routeSE050DecryptData);
#endif
  }
}