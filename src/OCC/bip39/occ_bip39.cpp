#include "occ_bip39.h"
#include "../utils/utils.h"


/**
 * Generate mnemonic from bytes
 *
 * @param string(0) <optional> byte array to be used for mnemonic generation
 * @return Generated mnemonic
 */
void routeBip39Mnemonic(OSCMessage &msg, int addressOffset)
{
    uint8_t data[32] = {0};
    
    if (msg.isString(0))
    {
        int length = msg.getDataLength(0);
        char hexStr[length];
        msg.getString(0, hexStr, length);

        memcpy(data, (const unsigned char *)fromhex(hexStr), 32);
    }else
        esp_fill_random(data, 32);
    
    OSCMessage resp_msg("/bip39Mnemonic");
    resp_msg.add(mnemonic_from_data(data, 24));

    sendOSCMessage(resp_msg);
}


/**
 * Generate seed from mnemonic
 *
 * @param string(0) Mnemonic
 * @return Generated seed
 */
void routeBip39MnemonicToSeed(OSCMessage &msg, int addressOffset)
{
    int res;
    size_t len;
    uint8_t seed[64];
    char passPhrase[128];
    OSCMessage resp_msg("/bip39MnemonicToSeed");

    if (msg.isString(0))
    {
        int length = msg.getDataLength(0);
        char mnemonic[length];
        msg.getString(0, mnemonic, length);

        if (msg.isString(1)){
            length = msg.getDataLength(1);
            msg.getString(1, passPhrase, length);
        }else{
            memcpy(passPhrase, "trustanchor", sizeof("trustanchor"));
        }

        mnemonic_to_seed(mnemonic, passPhrase, seed, NULL);

        String hexStr;
        hexStr = toHex(seed, 64);
        resp_msg.add(hexStr.c_str());
    }else{
        resp_msg.add("ERROR! No mnemonic");
    }

    sendOSCMessage(resp_msg);
}

