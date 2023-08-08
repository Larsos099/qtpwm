#ifndef ENCRYPT_H
#define ENCRYPT_H
#define KEYLENGTH 24
#include <OpenSSL/evp.h>
#include <OpenSSL/aes.h>
#include <QDebug>

bool encrypt192(unsigned char keydata[KEYLENGTH], unsigned char* salt, unsigned char* data, int length) {
    unsigned char key[KEYLENGTH], iv[KEYLENGTH];

    // Copy the original key and then pad it
    memcpy(key, keydata, KEYLENGTH);
    for (unsigned long i = KEYLENGTH; i < sizeof(key); ++i) {
        key[i] = 0;
    }

    int k_nf = EVP_BytesToKey(EVP_aes_192_cbc(), EVP_sha1(), salt, key, KEYLENGTH, 8, key, iv);
    if (k_nf != 24) {
        qWarning("KEY IS INVALID SIZE");
        return false;
    }

    EVP_CIPHER_CTX* e;
    EVP_CIPHER_CTX_init(e);
    EVP_EncryptInit(e, EVP_aes_192_cbc(), key, iv);

    return true;
}

#endif // ENCRYPT_H
