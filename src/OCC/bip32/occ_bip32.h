#pragma once

#include <OSCMessage.h>

extern "C"{
    #include "bip32.h"
    #include "base58.h"
    #include "curves.h"
}

#include <SLIPEncodedSerial.h>
extern SLIPEncodedSerial SLIPSerial;

#define  BIP32_SERIAL               SLIPSerial
#define  BIP3X_SERIAL_BEGIN()       SLIPSerial.beginPacket()
#define  BIP3X_SERIAL_END()         SLIPSerial.endPacket()

void routeBip32KeyFromSeed(OSCMessage &msg, int addressOffset);

