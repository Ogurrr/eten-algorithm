#include <iostream>
#include <string>
#include <openssl/rc4.h>

// Function to perform RC4 encryption
std::string encryptRC4(const std::string& plaintext, const std::string& key) {
    RC4_KEY rc4Key;
    RC4_set_key(&rc4Key, key.length(), reinterpret_cast<const unsigned char*>(key.c_str()));

    std::string ciphertext = plaintext;
    RC4(&rc4Key, ciphertext.length(), reinterpret_cast<unsigned char*>(&ciphertext[0]), reinterpret_cast<unsigned char*>(&ciphertext[0]));

    return ciphertext;
}

// Function to perform RC4 decryption
std::string decryptRC4(const std::string& ciphertext, const std::string& key) {
    RC4_KEY rc4Key;
    RC4_set_key(&rc4Key, key.length(), reinterpret_cast<const unsigned char*>(key.c_str()));

    std::string plaintext = ciphertext;
    RC4(&rc4Key, plaintext.length(), reinterpret_cast<unsigned char*>(&plaintext[0]), reinterpret_cast<unsigned char*>(&plaintext[0]));

    return plaintext;
}