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
    
    int hashASCII = stoi(toBinaryString(hash));
    int poolKeyASCII = stoi(toBinaryString(poolKey));
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

                    for (int m = 0; m < difficult * difficult * 10; ++m) {   
                        hash = bitwise_xor(hash, poolKey);
                        hash = bitwise_or(hash, poolKey);
                        hash = bitwise_and(hash, poolKey);
                        hash = caesarEncrypt(hash, stoi(toBinaryString(poolKey)));

                        hash = toBinaryString(hash);
                        poolKey = toBinaryString(poolKey);
                        poolKeyBinarySplit = splitBinary(poolKey);
        
                        poolKey = encode(poolKeyBinarySplit);
                        hash = encode(hashBinarySplit);
                        hash = caesarEncrypt(hash, stoi(toBinaryString(poolKey)));
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
    }
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
    hash = sha512(hash);
    hash = MD5(hash);
    hash = MD5(hash);
    hash = caesarEncrypt(hash, stoi(toBinaryString(poolKey)));

    poolKey = bitwise_and(poolKey, hash);
    poolKey = bitwise_nand(poolKey, hash);
    poolKey = bitwise_xor(poolKey, hash);
    poolKey = bitwise_or(poolKey, hash);
    poolKey = sha512(poolKey);
    poolKey = sha512(poolKey);
    poolKey = MD5(poolKey);
    poolKey = MD5(poolKey);
    poolKey = caesarEncrypt(poolKey, stoi(toBinaryString(hash)));

    return hash;
}