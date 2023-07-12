#pragma once

#include "OSCMessage.h"

extern "C"{
    #include "bip39.h"
}

#include <SLIPEncodedSerial.h>
extern SLIPEncodedSerial SLIPSerial;

#define  BIP32_SERIAL               SLIPSerial
#define  BIP3X_SERIAL_BEGIN()       SLIPSerial.beginPacket()
#define  BIP3X_SERIAL_END()         SLIPSerial.endPacket()


void routeBip39Mnemonic(OSCMessage &msg, int addressOffset);
void routeBip39MnemonicToSeed(OSCMessage &msg, int addressOffset);

