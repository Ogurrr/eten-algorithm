#include <iostream>
#include <string>
#include <sstream>
#include <bitset>
#include <vector>
#include <cmath>
#include <thread>
#include <cstring>

#include "logic.h"
#include "base64.h"
#include "utils.h"
#include "railfence.h"
#include "sha.h"
#include "md.h"
#include "RC4.h"
#include "caesar.h"
std::string hashToHex(unsigned long hash) {
    std::stringstream ss;
    ss << std::hex << hash;
    return ss.str();
}

// Główna funkcja obliczająca
std::string etenCompute(std::string hash, std::string poolKey, int difficult) {
    std::string hashStarting = hash;
    hash = bitwise_xor(hash, poolKey);
    hash = bitwise_or(hash, poolKey);
    hash = bitwise_and(hash, poolKey);
    hash = bitwise_nand(hash, poolKey);

    hash = bitwise_xor(hash, poolKey);
    std::vector<std::string> hashBinarySplit = splitBinary(hash);
    hash = encode(hashBinarySplit);

    std::vector<std::string> poolKeyBinarySplit = splitBinary(poolKey);
    poolKey = encode(poolKeyBinarySplit);
    hash = encode(hashBinarySplit);

    hash = toBinaryString(hash);
    poolKey = toBinaryString(poolKey);

    hash = bitwise_xor(hash, poolKey);
    hash = bitwise_xor(hash, poolKey);

    for (int i = 0; i < difficult; ++i) {
        hash = bitwise_xor(hash, poolKey);
        hash = bitwise_or(hash, poolKey);
        hash = bitwise_and(hash, poolKey);

        hash = toBinaryString(hash);
        poolKey = toBinaryString(poolKey);
        poolKeyBinarySplit = splitBinary(poolKey);

        poolKey = encode(poolKeyBinarySplit);
        hash = encode(hashBinarySplit);

        hash = sha512(hash);
        poolKey = sha512(poolKey);

        hash = sha256(hash);
        poolKey = sha256(poolKey);
    } 

    hash = bitwise_xor(hash, poolKey);
    hash = bitwise_or(hash, poolKey);
    hash = bitwise_and(hash, poolKey);
    hash = bitwise_nand(hash, poolKey);

    for (int i = 0; i < difficult; ++i) {
        hash = bitwise_xor(hash, poolKey);
        hash = bitwise_or(hash, poolKey);
        hash = bitwise_and(hash, poolKey);

        hash = toBinaryString(hash);
        poolKey = toBinaryString(poolKey);
        poolKeyBinarySplit = splitBinary(poolKey);

        poolKey = encode(poolKeyBinarySplit);
        hash = encode(hashBinarySplit);

        hash = sha512(hash);
        poolKey = sha512(poolKey);

        hash = sha256(hash);
        poolKey = sha256(poolKey);
        hash = MD5(hash);
        poolKey = MD5(poolKey);
        hash = sha512(hash);
        poolKey = sha512(poolKey);
        poolKey = encryptRC4(poolKey, hash);
        hash = encryptRC4(hash, poolKey);
    }

    hash = bitwise_xor(hash, poolKey);
    hash = bitwise_or(hash, poolKey);
    hash = bitwise_and(hash, poolKey);
    hash = toBinaryString(hash);
    poolKey = toBinaryString(poolKey);
    poolKeyBinarySplit = splitBinary(poolKey);
    hashBinarySplit = splitBinary(hash);

    poolKey = encode(poolKeyBinarySplit);
    hash = encode(hashBinarySplit);
    hash = sha256(hash);
    poolKey = sha256(poolKey);
    hash = MD5(hash);
    poolKey = MD5(poolKey);

    hash = bitwise_xor(hash, poolKey);
    hash = bitwise_or(hash, poolKey);
    hash = bitwise_and(hash, poolKey);
    hash = bitwise_nand(hash, poolKey);
    hash = sha512(hash);
    poolKey = sha512(poolKey);

    poolKey = encode(poolKeyBinarySplit);
    hash = encode(hashBinarySplit);
    hash = sha256(hash);
    poolKey = sha256(poolKey);
    hash = MD5(hash);
    poolKey = MD5(poolKey);

    hash = bitwise_xor(hash, poolKey);
    hash = bitwise_or(hash, poolKey);
    hash = bitwise_and(hash, poolKey);
    hash = bitwise_nand(hash, poolKey);
    hash = sha512(hash);
    poolKey = sha512(poolKey);

    hash = bitwise_and(hash, poolKey);
    hash = bitwise_nand(hash, poolKey);

    for (int i = 0; i < difficult * difficult; ++i) {
        hash = bitwise_xor(hash, poolKey);
        hash = bitwise_or(hash, poolKey);
        hash = bitwise_and(hash, poolKey);

        hash = toBinaryString(hash);
        poolKey = toBinaryString(poolKey);
        poolKeyBinarySplit = splitBinary(poolKey);

        poolKey = encode(poolKeyBinarySplit);
        hash = encode(hashBinarySplit);

        hash = sha512(hash);
        poolKey = sha512(poolKey);

        hash = sha256(hash);
        poolKey = sha256(poolKey);
        hash = MD5(hash);
        poolKey = MD5(poolKey);
        hash = sha512(hash);
        poolKey = sha512(poolKey);
        poolKey = encryptRC4(poolKey, hash);
        hash = encryptRC4(hash, poolKey);

        for (int j = 0; j < difficult ; ++j) {
            hash = bitwise_xor(hash, poolKey);
            hash = bitwise_or(hash, poolKey);
            hash = bitwise_and(hash, poolKey);

            hash = toBinaryString(hash);
            poolKey = toBinaryString(poolKey);
            poolKeyBinarySplit = splitBinary(poolKey);

            poolKey = encode(poolKeyBinarySplit);
            hash = encode(hashBinarySplit);

            hash = sha512(hash);
            poolKey = sha512(poolKey);

            hash = sha256(hash);
            poolKey = sha256(poolKey);
            hash = MD5(hash);
            poolKey = MD5(poolKey);
            hash = sha512(hash);
            poolKey = sha512(poolKey);
            poolKey = encryptRC4(poolKey, hash);
            hash = encryptRC4(hash, poolKey);
        }
    }
    hash = MD5(hash);
    unsigned long tmp = 0;
    for (char c : hash) {
        tmp += static_cast<unsigned long>(c);
    }

    // Konwersja hasza na heksadecymalny string
    std::stringstream ss;
    ss << std::hex << tmp;
    std::string hexHash = ss.str();

    std::string result = hexHash;
    while (result.length() < 8) {
        result += hexHash;
    }
    hash = sha512(result);
    hash = MD5(hash);
    hash = caesarEncrypt(hash,8);
    hash = encode(hashBinarySplit);
    hash = caesarEncrypt(hash,stoi(toBinaryString(hashStarting)));
    hash = encode(hashBinarySplit);
    hash = caesarEncrypt(hash,stoi(toBinaryString(hashStarting)));
    hash = bitwise_xor(hash, hashStarting);
    hash = bitwise_or(hash, hashStarting);
    hash = bitwise_and(hash, hashStarting);
    tmp = 0;
    for (char c : hash) {
        tmp += static_cast<unsigned long>(c);
    }

    // Konwersja hasza na heksadecymalny string
    ss << std::hex << tmp;
    hexHash = ss.str();

    result = hexHash;
    while (result.length() < 8) {
        result += hexHash;
    }
    hash = sha512(result);
    hash = MD5(hash);
    hash = encryptRailFence(hash,8);
    hash = caesarEncrypt(hash,8);
    hash = encode(hashBinarySplit);
    hash = caesarEncrypt(hash,stoi(toBinaryString(hashStarting)));
    hash = encode(hashBinarySplit);
    hash = caesarEncrypt(hash,stoi(toBinaryString(hashStarting)));
    return hash.substr(0, 16);

}
std::string etenCalc(std::string hash, std::string poolKey, int difficult,int size) {
    if(hash.length() != poolKey.length()) {
        return "-1";
    }
    int count = size / 16;
    std::vector<std::string> hashChunks;
    std::vector<std::string> poolKeyChunks;
    std::string out = "";
    size_t hashLength = hash.length();

    for (size_t i = 0; i < count; ++i) {
        hashChunks.push_back(hash.substr(i, 1));
        poolKeyChunks.push_back(poolKey.substr(i, 1));
    }

    for (size_t i = 0; i < count; ++i) {
        out += etenCompute(hashChunks[i], poolKeyChunks[i], difficult);
    } 
    if (size >= 0 && size < out.length()) {
        out.erase(size);
    }
    return out;
}