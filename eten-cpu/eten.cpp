#include <iostream>
#include <string>
#include <sstream> // for std::stringstream
#include <bitset>
#include <vector>
#include <math.h>

#include "logic.h"
#include "base64.h"
#include "utils.h"
#include "railfence.h"
#include "sha.h"
#include "md.h"
#include "RC4.h"
#include "ceasar.h"
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
    for(int i; i < difficult * difficult * 50; i++)
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
        for(int i; i < difficult * difficult * 200; i++)
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
            for(int i; i < difficult * difficult * 10; i++)
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
                for(int i; i < difficult * difficult * 10; i++)
                {      
                    hash = bitwise_xor(hash,poolKey);
                    hash = bitwise_or(hash,poolKey);
                    hash = bitwise_and(hash,poolKey);
                    hash = caesarEncrypt(hash,stoi(toBinaryString(poolKey))) {

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
                    for(int i; i < difficult * difficult * 10; i++)
                    {   
                        hash = bitwise_xor(hash,poolKey);
                        hash = bitwise_or(hash,poolKey);
                        hash = bitwise_and(hash,poolKey);
                        hash = caesarEncrypt(hash,stoi(toBinaryString(poolKey)));

                        hash = toBinaryString(hash);
                        poolKey = toBinaryString(poolKey);
                        poolKeyBinarySplit = splitBinary(poolKey);
        
                        poolKey = encode(poolKeyBinarySplit);
                        hash = encode(hashBinarySplit);
                        hash = caesarEncrypt(hash,stoi(toBinaryString(poolKey)));
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

    hash = bitwise_xor(hash,poolKey);
    hash = bitwise_or(hash,poolKey);
    hash = bitwise_and(hash,poolKey);
    hash = bitwise_nand(hash,poolKey);
    hash = sha512(hash);
    hash = sha512(hash);
    hash = md5(hash);
    hash = md5(hash);
    hash = caesarEncrypt(hash,stoi(toBinaryString(poolKey)));

    poolKey = bitwise_and(poolKey,hash);
    poolKey = bitwise_nand(poolKey,hash);
    poolKey = bitwise_xor(poolKey,hash);
    poolKey = bitwise_or(poolKey,hash);
    poolKey = sha512(poolKey);
    poolKey = sha512(poolKey);
    poolKey = md5(poolKey);
    poolKey = md5(poolKey);
    poolKey = caesarEncrypt(poolKey,stoi(toBinaryString(hash)));

    return hash;
};

