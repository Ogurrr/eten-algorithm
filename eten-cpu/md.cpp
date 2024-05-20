#include <iostream>
#include <openssl/md5.h>
#include <openssl/md2.h>
#include <openssl/md4.h>
#include <openssl/ripemd.h>
#include <string>
#include <iomanip>
#include <sstream>

const int MD2_DIGEST_LENGTH = 16;

// Function to compute MD5 hash of a string
std::string computeMD5(const std::string& input) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(input.c_str()), input.length(), digest);

    std::stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }

    return ss.str();
}

std::string computeMD2(const std::string& input) {
    unsigned char digest[MD2_DIGEST_LENGTH];
    MD2_CTX context;
    MD2_Init(&context);
    MD2_Update(&context, reinterpret_cast<const unsigned char*>(input.c_str()), input.length());
    MD2_Final(digest, &context);

    std::stringstream ss;
    for (int i = 0; i < MD2_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }

    return ss.str();
}

std::string computeMD4(const std::string& input) {
    unsigned char digest[MD4_DIGEST_LENGTH];
    MD4_CTX context;
    MD4_Init(&context);
    MD4_Update(&context, reinterpret_cast<const unsigned char*>(input.c_str()), input.length());
    MD4_Final(digest, &context);

    std::stringstream ss;
    for (int i = 0; i < MD4_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }

    return ss.str();
}

std::string computeRIPEMD160(const std::string& input) {
    unsigned char digest[RIPEMD160_DIGEST_LENGTH];
    RIPEMD160_CTX context;
    RIPEMD160_Init(&context);
    RIPEMD160_Update(&context, reinterpret_cast<const unsigned char*>(input.c_str()), input.length());
    RIPEMD160_Final(digest, &context);

    std::stringstream ss;
    for (int i = 0; i < RIPEMD160_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }

    return ss.str();
}