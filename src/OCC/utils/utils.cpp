#include "utils.h"

#define BIP32_INITIAL_HARDENED_CHILD 0x80000000


void sendOSCMessage(OSCMessage& resp_msg){
    ESP_SERIAL_BEGIN();
    resp_msg.send(ESP_SERIAL);
    ESP_SERIAL_END();
    resp_msg.empty();
    delay(20);
}


// forward declaration
void err(const char * message, void * data = NULL);

#define FROMHEX_MAXLEN 1024

void memzero(void *const pnt, const size_t len) {

  volatile unsigned char *volatile pnt_ = (volatile unsigned char *volatile)pnt;
  size_t i = (size_t)0U;

  while (i < len) {
    pnt_[i++] = 0U;
  }
}

const uint8_t *fromhex(const char *str) {
  static uint8_t buf[FROMHEX_MAXLEN];
  size_t len = strlen(str) / 2;
  if (len > FROMHEX_MAXLEN) len = FROMHEX_MAXLEN;
  for (size_t i = 0; i < len; i++) {
    uint8_t c = 0;
    if (str[i * 2] >= '0' && str[i * 2] <= '9') c += (str[i * 2] - '0') << 4;
    if ((str[i * 2] & ~0x20) >= 'A' && (str[i * 2] & ~0x20) <= 'F')
      c += (10 + (str[i * 2] & ~0x20) - 'A') << 4;
    if (str[i * 2 + 1] >= '0' && str[i * 2 + 1] <= '9')
      c += (str[i * 2 + 1] - '0');
    if ((str[i * 2 + 1] & ~0x20) >= 'A' && (str[i * 2 + 1] & ~0x20) <= 'F')
      c += (10 + (str[i * 2 + 1] & ~0x20) - 'A');
    buf[i] = c;
  }
  return buf;
}

void tohexprint(char *hexbuf, uint8_t *str, int strlen){
   // char hexbuf[strlen];
    for (int i = 0 ; i < strlen/2 ; i++) {
        sprintf(&hexbuf[2*i], "%02X", str[i]);
    }
  hexbuf[strlen-2] = '\0';
}

size_t toHex(const uint8_t * array, size_t arraySize, char * output, size_t outputSize){
    if(array == NULL || output == NULL){ return 0; }
    // uint8_t * array = (uint8_t *) arr;
    if(outputSize < 2*arraySize){
        return 0;
    }
    memzero(output, outputSize);

    for(size_t i=0; i < arraySize; i++){
        output[2*i] = (array[i] >> 4) + '0';
        if(output[2*i] > '9'){
            output[2*i] += 'a'-'9'-1;
        }

        output[2*i+1] = (array[i] & 0x0F) + '0';
        if(output[2*i+1] > '9'){
            output[2*i+1] += 'a'-'9'-1;
        }
    }
    return 2*arraySize;
}

String toHex(const uint8_t * array, size_t arraySize){
    if(array == NULL){ return String(); }
    size_t outputSize = arraySize * 2 + 1;
    char * output = (char *) malloc(outputSize);
    if(output == NULL){ return String(); }

    toHex(array, arraySize, output, outputSize);

    String result(output);

    memzero(output, outputSize);
    free(output);

    return result;
}


std::vector<uint32_t> getPath(char* pathStr){
	std::vector<uint32_t> myPath;
  uint32_t num = 0;
	char *token = strtok(pathStr, "/");
	token = strtok(NULL, "/");
	while (token != NULL)
	{
		size_t s = strlen(token);
		char   t = token[s-1];
		if(t == '\''){
			token[s-1] = '\0';
			num = BIP32_INITIAL_HARDENED_CHILD;
		}else 
			num = 0;

		num += std::stoi(token);
		myPath.push_back(num);

		token = strtok(NULL, "/");
	}
		
	return myPath;
}