#include <iostream>
#include <openssl/md5.h>
#include <openssl/md2.h>
#include <openssl/md4.h>
#include <openssl/ripemd.h>
#include <string>
#include <iomanip>

// Function to compute MD5 hash of a string
std::string MD5(const std::string& input) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(input.c_str()), input.length(), digest);

    std::stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }

    return ss.str();
}

std::string MD2(const std::string& input) {
    unsigned char digest[MD2_DIGEST_LENGTH];
    MD2(reinterpret_cast<const unsigned char*>(input.c_str()), input.length(), digest);

    std::stringstream ss;
    for (int i = 0; i < MD2_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }

    return ss.str();
}

std::string MD4(const std::string& input) {
    unsigned char digest[MD4_DIGEST_LENGTH];
    MD4(reinterpret_cast<const unsigned char*>(input.c_str()), input.length(), digest);

    std::stringstream ss;
    for (int i = 0; i < MD4_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }

    return ss.str();
}

std::string RIPEMD160(const std::string& input) {
    unsigned char digest[RIPEMD160_DIGEST_LENGTH];
    RIPEMD160(reinterpret_cast<const unsigned char*>(input.c_str()), input.length(), digest);

    std::stringstream ss;
    for (int i = 0; i < RIPEMD160_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }

    return ss.str();
}