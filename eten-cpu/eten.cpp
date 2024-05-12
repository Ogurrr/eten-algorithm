#include <iostream>
#include <string>
#include <sstream> // for std::stringstream
#include <bitset>
#include <vector>

#include "logic.h"
#include "base64.h"
#include "utils.h"
#include "railfence.h"
#include "sha.h"
#include "md.h"
#include "RC4.h"

int stringToAscii(std::string input)
{
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
	string tmp = ss.str();
	s = stoi(tmp);
    return s;
}

std::string etenCalc(std::string hash,std::string poolKey,int difficult)
{
    hash = bitwise_xor(hash,poolKey);
    hash = bitwise_or(hash,poolKey);
    hash = bitwise_and(hash,poolKey);
       hash = bitwise_nand(hash,poolKey);

    hash = bitwise_xor(hash,poolKey);
    vector<string> hashBinarySplit = splitBinary(hash);
    hash = encode(hashBinarySplit);

    vector<string> poolKeyBinarySplit = splitBinary(poolKey);
    poolKey = encode(poolKeyBinarySplit);
    hash = encode(hashBinarySplit);
    
    hash = toBinaryString(hash);
    poolKey = toBinaryString(poolKey);
    
    hash = bitwise_xor(hash,poolKey);
    hash = bitwise_xor(hash,poolKey);
    for(int i; i < difficult; i++)
    {
        hash = bitwise_xor(hash,poolKey);
        hash = bitwise_or(hash,poolKey);
        hash = bitwise_and(hash,poolKey);
        
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
    hash = encryptRailFence(hash,poolKeyASCII);
    poolKey = encryptRailFence(poolKey,hashASCII);
    hash = bitwise_xor(hash,poolKey);
    hash = bitwise_or(hash,poolKey);
    hash = bitwise_and(hash,poolKey);
       hash = bitwise_nand(hash,poolKey);
    for(int i; i < difficult; i++)
    {
        hash = bitwise_xor(hash,poolKey);
        hash = bitwise_or(hash,poolKey);
        hash = bitwise_and(hash,poolKey);
        
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
        poolKey = encryptRC4(poolKey,hash);
        hash = encryptRC4(hash,poolKey);

    }
    hash = bitwise_xor(hash,poolKey);
    hash = bitwise_or(hash,poolKey);
    hash = bitwise_and(hash,poolKey);
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

    hash = bitwise_xor(hash,poolKey);
    hash = bitwise_or(hash,poolKey);
    hash = bitwise_and(hash,poolKey);
    hash = bitwise_nand(hash,poolKey);
    hash = sha512(hash);
    poolKey = sha512(poolKey);

    poolKey = encode(poolKeyBinarySplit);
    hash = encode(hashBinarySplit);
    hash = sha256(hash);
    poolKey = sha256(poolKey);
    hash = MD5(hash);
    poolKey = MD5(poolKey);

    hash = bitwise_xor(hash,poolKey);
    hash = bitwise_or(hash,poolKey);
    hash = bitwise_and(hash,poolKey);
    hash = bitwise_nand(hash,poolKey);
    hash = sha512(hash);
    poolKey = sha512(poolKey);

    hash = bitwise_and(hash,poolKey);
    hash = bitwise_nand(hash,poolKey);
    return hash;
};
