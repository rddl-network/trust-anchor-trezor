#include "valise.h"
#include "../utils/utils.h"
#include <Preferences.h>


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