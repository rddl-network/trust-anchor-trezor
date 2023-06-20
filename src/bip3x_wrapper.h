#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
#include <OSCBoards.h>

extern "C"{
    #include "bip32.h"
    #include "bip39.h"
    #include "base58.h"
    #include "curves.h"
}

#include <SLIPEncodedSerial.h>
extern SLIPEncodedSerial SLIPSerial;

#define  BIP32_SERIAL               SLIPSerial
#define  BIP3X_SERIAL_BEGIN()       SLIPSerial.beginPacket()
#define  BIP3X_SERIAL_END()         SLIPSerial.endPacket()

void routeValiseSeedSet(OSCMessage &msg, int addressOffset);
void routeValiseSeedGet(OSCMessage &msg, int addressOffset);
void routeBip39Mnemonic(OSCMessage &msg, int addressOffset);
void routeBip39MnemonicToSeed(OSCMessage &msg, int addressOffset);
void routeBip32KeyFromSeed(OSCMessage &msg, int addressOffset);
void routeSignDigest(OSCMessage &msg, int addressOffset);
void routeVerifySign(OSCMessage &msg, int addressOffset);
void bip3xOSCMsgMap(OSCMessage& msg);