#include "bip3x_wrapper.h"
#include "utils.h"
#include <Preferences.h>

void sendOSCMessage(OSCMessage& resp_msg){
    BIP3X_SERIAL_BEGIN();
    resp_msg.send(BIP32_SERIAL);
    BIP3X_SERIAL_END();
    resp_msg.empty();
    delay(20);
}

Preferences valise;

/**
 * Store the base seed inside the trust anchor's memory
 *
 * @param String(0) The base seed.
 * @param String(2) empty string for future use
 * @return  Generated '0' or '1' string for failure or success. Sending over OSC as string

 */
void routeValiseSeedSet(OSCMessage &msg, int addressOffset)
{
    int res;

    char char_seed[129];

    if (msg.isString(0))
    {
        int length = msg.getDataLength(0);
        msg.getString(0, char_seed, length);
    }

    valise.begin("vault", false);
    valise.putString("valise_seed", (const char *)char_seed);
    valise.end();

    OSCMessage resp_msg("/valiseSeedSet");
    resp_msg.add("1");

    sendOSCMessage(resp_msg);

    delay(20);
}

/**
 * Get the base seed from the trust anchor's memory
 *
 * @param String(0) empty string for future use
 * @return The stored base seed. Sending over OSC as string
.
 */
void routeValiseSeedGet(OSCMessage &msg, int addressOffset)
{
    valise.begin("vault", false);
    String valise_seed = valise.getString("valise_seed", "");
    valise.end();

    OSCMessage resp_msg("/valiseSeedGet");
    resp_msg.add(valise_seed.c_str());

    sendOSCMessage(resp_msg);

    delay(20);
}

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




/**
 * Generate priv and pub key from given Seed
 *
 * @param string(0) seed in String type
 * @return Generated priv and pub key in Base58. Sending over OSC as string
 */
void routeBip32KeyFromSeed(OSCMessage &msg, int addressOffset)
{
    OSCMessage resp_msg("/bip32KeyFromSeed");
    HDNode hdnode;
    uint8_t sign[64]; 
    
    if (msg.isString(0))
    {
        int length = msg.getDataLength(0);
        char seed[length];
        msg.getString(0, seed, length);

        int resp = hdnode_from_seed((const unsigned char *)fromhex(seed), length/2, ED25519_NAME, &hdnode);

        ed25519_publickey(hdnode.private_key, hdnode.public_key);

        char priv_b58[256];
        char pub_b58[256];
        size_t priv_len, pub_len;
        b58enc(priv_b58, &priv_len, hdnode.private_key, 32);
        b58enc(pub_b58, &pub_len, hdnode.public_key, 33);

        resp_msg.add(priv_b58);
        resp_msg.add(pub_b58);
    }

    sendOSCMessage(resp_msg);
}


/**
 * Signning Message with the key generated by using the seed placed on Flash Memory.
 * If there is no seed on flash, it will fail. Write seed by using valiseSeedSet.
 * 
 * @param string(0) Message to be signed 
 * @return Signed message
 */
void routeSignDigest(OSCMessage &msg, int addressOffset)
{
    OSCMessage resp_msg("/SignDigest");
    HDNode hdnode;
    uint8_t sign[64]; 

    valise.begin("vault", false);
    String valise_seed = valise.getString("valise_seed", "");
    valise.end();

    if(!valise_seed.isEmpty() & msg.isString(0)){
        int resp = hdnode_from_seed((const uint8_t*)fromhex(valise_seed.c_str()), valise_seed.length()/2, ED25519_NAME, &hdnode);
        
        int length = msg.getDataLength(0);
        char message[length];
        msg.getString(0, message, length);

        resp = hdnode_sign(&hdnode, (const uint8_t*)message, length, HASHER_SHA2, sign, NULL, NULL);

        String hexStr;
        hexStr = toHex((uint8_t*)&sign, sizeof(sign));
        resp_msg.add(hexStr.c_str());

    }else{
        resp_msg.add("ERROR! First Generate Seed");
    }

    sendOSCMessage(resp_msg);
}

/**
 * Verifying Message with the key generated by using the seed placed on Flash Memory.
 * If there is no seed on flash, it will fail. Write seed by using valiseSeedSet.
 * 
 * @param string(0) Message 
 * @param string(1) Sign of the Message 
 * @return If success, signed message. ERROR message if fail
 */
void routeVerifySign(OSCMessage &msg, int addressOffset)
{
    OSCMessage resp_msg("/VerifyDigest");
    HDNode hdnode;
    char sign[256];

    valise.begin("vault", false);
    String valise_seed = valise.getString("valise_seed", "");
    valise.end();


    if(!valise_seed.isEmpty() & msg.isString(0) & msg.isString(1)){
        int resp = hdnode_from_seed((const uint8_t*)fromhex(valise_seed.c_str()), valise_seed.length()/2, ED25519_NAME, &hdnode);

        ed25519_publickey(hdnode.private_key, hdnode.public_key);

        int msgLen = msg.getDataLength(0);
        char message[msgLen];
        msg.getString(0, message, msgLen);

        int signLen = msg.getDataLength(1);
        char signStr[signLen];
        msg.getString(1, signStr, signLen);

        if(ed25519_sign_open((const uint8_t*)message, msgLen, hdnode.public_key, fromhex(signStr)) != 0){
            resp_msg.add("ERROR! Verifying Signature");
        }else{ 
            resp_msg.add(signStr);
        }
    }else{
        resp_msg.add("ERROR! Wrong parameters");
    }

    sendOSCMessage(resp_msg);
}


void bip3xOSCMsgMap(OSCMessage& msg){
    msg.route("/IHW/bip39Mnemonic", routeBip39Mnemonic);
    msg.route("/IHW/bip39MnemonicFromBytes", routeBip39Mnemonic);
    msg.route("/IHW/bip39MnemonicToSeed", routeBip39MnemonicToSeed);
    msg.route("/IHW/bip32_key_from_seed", routeBip32KeyFromSeed);
    msg.route("/IHW/valiseSeedSet", routeValiseSeedSet);
    msg.route("/IHW/valiseSeedGet", routeValiseSeedGet);
    msg.route("/IHW/valiseSignDigest", routeSignDigest);
    msg.route("/IHW/valiseVerifySign", routeVerifySign);
}