#pragma once

#include <iostream>
#include <vector>
#include <WString.h>
#include <string.h>
#include <sstream>
#include "OSCMessage.h"
#include "SLIPEncodedSerial.h"

extern SLIPEncodedSerial SLIPSerial;

#define  ESP_SERIAL               SLIPSerial
#define  ESP_SERIAL_BEGIN()       SLIPSerial.beginPacket()
#define  ESP_SERIAL_END()         SLIPSerial.endPacket()

void sendOSCMessage(OSCMessage& resp_msg);
void memzero(void *const pnt, const size_t len);
const uint8_t *fromhex(const char *str);
void tohexprint(char *hexbuf, uint8_t *str, int strlen);
size_t toHex(const uint8_t * array, size_t arraySize, char * output, size_t outputSize);
String toHex(const uint8_t * array, size_t arraySize);
std::vector<uint32_t> getPath(char* pathStr);