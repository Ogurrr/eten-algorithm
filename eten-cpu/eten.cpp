#include <iostream>
#include <string>
#include <sstream>
#include <bitset>
#include <vector>
#include <cmath>

#include "logic.h"
#include "base64.h"
#include "utils.h"
#include "railfence.h"
#include "sha.h"
#include "md.h"
#include "RC4.h"
#include "caesar.h"

// Funkcja pomocnicza do konwersji stringa na ASCII
int stringToAscii(const std::string& input) {
    std::stringstream ss;
    int s;
    std::vector<int> ascii_codes;

    for (char c : input) {
        ascii_codes.push_back(static_cast<int>(c));
    }

    ss << ascii_codes[0];
    for (size_t i = 1; i < ascii_codes.size(); ++i) {
        ss << " " << ascii_codes[i];
    }
    std::string tmp = ss.str();
    s = stoi(tmp);
    return s;
}

// Główna funkcja obliczająca
std::string etenCalc(std::string hash, std::string poolKey, int difficult) {
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

    int hashASCII = stringToAscii(hash);
    int poolKeyASCII = stringToAscii(poolKey);
    hash = encryptRailFence(hash, poolKeyASCII);
    poolKey = encryptRailFence(poolKey, hashASCII);

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

    for (int i = 0; i < difficult * difficult * 50; ++i) {
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

        for (int j = 0; j < difficult * difficult * 200; ++j) {
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

            for (int k = 0; k < difficult * difficult * 10; ++k) {
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

                for (int l = 0; l < difficult * difficult * 10; ++l) {
                    hash = bitwise_xor(hash, poolKey);
                    hash = bitwise_or(hash, poolKey);
                    hash = bitwise_and(hash, poolKey);
                    hash = caesarEncrypt(hash, stoi(toBinaryString(poolKey)));

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
        }
    }

    return hash;
}
