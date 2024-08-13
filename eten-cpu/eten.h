#include <iostream>

std::string etenCompute(std::string hash,std::string poolKey,int difficult);
extern "C" {
    std::string etenCalc(std::string hash, std::string poolKey, int difficult,int size);
}