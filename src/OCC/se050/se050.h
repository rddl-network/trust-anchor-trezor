#pragma once

#ifdef DSE050
#include "OSCMessage.h"

extern "C"{
    #include "bip32.h"
    #include "aes/aes.h"
}

#include <SLIPEncodedSerial.h>
extern SLIPEncodedSerial SLIPSerial;

#define  BIP32_SERIAL               SLIPSerial
#define  BIP3X_SERIAL_BEGIN()       SLIPSerial.beginPacket()
#define  BIP3X_SERIAL_END()         SLIPSerial.endPacket()

void routeSE050EncryptData(OSCMessage &msg, int addressOffset);
void routeSE050DecryptData(OSCMessage &msg, int addressOffset);
#endif