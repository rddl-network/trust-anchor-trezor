#ifdef DSE050

#include "se050.h"
#include "../utils/utils.h"
#include "se050_middleware.h"

constexpr int BINARY_RW_SLOT = 224;
constexpr int BINARY_READ_SIZE = 64;
constexpr int DEFAULT_OVERRIDE_FLAG = 0; // 0: Dont override se050 slot, 1: Override
std::string AES_INIT_VECT {"f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff"};


std::vector<uint8_t> getAESEncrypt(const std::string plainp, const uint8_t* key){
    int plainTextLen = plainp.length()-1;
    std::vector<uint8_t> result(plainTextLen/2);
    aes_encrypt_ctx ctxe;
    aes_encrypt_key256(key, &ctxe);

    uint8_t cbuf[16];
    memcpy(cbuf, fromhex(AES_INIT_VECT.c_str()), 16);

    aes_ctr_encrypt(fromhex(plainp.data()), &result[0], plainTextLen/2, cbuf, aes_ctr_cbuf_inc, &ctxe);

    return result;
}


std::vector<uint8_t> getAESDecrypt(const std::vector<uint8_t>cipherp, const uint8_t* key){
    int chiperTextLen = cipherp.size();
    std::vector<uint8_t> result(chiperTextLen);
    aes_encrypt_ctx ctxe;
    aes_encrypt_key256(key, &ctxe);

    uint8_t cbuf[16];
    memcpy(cbuf, fromhex(AES_INIT_VECT.c_str()), 16);

    aes_ctr_decrypt(cipherp.data(), &result[0], cipherp.size(), cbuf, aes_ctr_cbuf_inc, &ctxe);

    return result;
}


/**
 * Write AES Encrypted data of given Seed into SE050
 *
 * @param string(0) Seed in String type
 * @param string(1) <optional> Override Flag. 0: Dont write, if there is any data on SE050
 *                                            1: Write anyway 
 * @param string(2) <optional> Key in String type
 * @return Status in string type
 */
void routeSE050EncryptData(OSCMessage &msg, int addressOffset) 
{
    OSCMessage resp_msg("/se050SetSeed");
    std::string key, plainTxt;
    uint8_t keyBuf[32];
    int overrideFlash = DEFAULT_OVERRIDE_FLAG;
    memset(keyBuf, 0, sizeof(keyBuf));
 
    if (msg.isString(0))
    {
        int kyLen = 0;
        int len = msg.getDataLength(0);
        char plainText[len];
        plainTxt.resize(len);
        msg.getString(0, &plainTxt[0], len);

        if (msg.isInt(1))
        {
           overrideFlash = msg.getInt(1);
        }

        if (msg.isString(2))
        {
            kyLen = msg.getDataLength(2);
            key.resize(kyLen);
            msg.getString(2, &key[0], kyLen);

            sha3_256(fromhex(key.data()), (key.size()-1)/2, keyBuf);
        }

        if(overrideFlash != 0)
            se050_obj.delete_obj(BINARY_RW_SLOT);

        std::vector<uint8_t> cipherArr = getAESEncrypt(plainTxt, keyBuf);

        if(se050_obj.write_binary_data(BINARY_RW_SLOT, cipherArr) == cipherArr.size())
            resp_msg.add("Binary Data Written");
        else
            resp_msg.add("ERROR! Write Binary Data");
        
    }

    sendOSCMessage(resp_msg);
}


/**
 * Read Seed from SE050. 
 * Always read BINARY_READ_SIZE bytes from se050.
 * 
 * @param string(0) <optional> Key in String type
 * @return Seed in string type
 */
void routeSE050DecryptData(OSCMessage &msg, int addressOffset) 
{
    OSCMessage resp_msg("/se050GetSeed");
    std::string key;
    uint8_t keyBuf[32];
    memset(keyBuf, 0, sizeof(keyBuf));

    if (msg.isString(0))
    {
        int kyLen = msg.getDataLength(0);
        key.resize(kyLen);
        msg.getString(0, &key[0], kyLen);

        sha3_256(fromhex(key.data()), (key.size()-1)/2, keyBuf);
    }

    auto cipherp = se050_obj.read_binary_data(BINARY_RW_SLOT, BINARY_READ_SIZE);

    auto plainArr = getAESDecrypt(cipherp, keyBuf);

    String hexStr;
    hexStr = toHex(plainArr.data(), plainArr.size());
    resp_msg.add(hexStr.c_str());

    sendOSCMessage(resp_msg);
}

#endif