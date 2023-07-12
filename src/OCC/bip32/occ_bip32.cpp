#include "occ_bip32.h"
#include "../utils/utils.h"
#include <Preferences.h>


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

