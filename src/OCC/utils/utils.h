#pragma once

#include <iostream>
#include <vector>
#include <WString.h>
#include <string.h>
#include <sstream>

void memzero(void *const pnt, const size_t len);
const uint8_t *fromhex(const char *str);
void tohexprint(char *hexbuf, uint8_t *str, int strlen);
size_t toHex(const uint8_t * array, size_t arraySize, char * output, size_t outputSize);
String toHex(const uint8_t * array, size_t arraySize);
std::vector<uint32_t> getPath(char* pathStr);