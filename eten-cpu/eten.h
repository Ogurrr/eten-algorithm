#include <iostream>
#ifndef ETENALGORITHM_H
#define ETENALGORITHM_H

#ifdef __cplusplus
extern "C" {
#endif
std::string etenCompute(std::string hash,std::string poolKey,int difficult);
std::string etenCalc(std::string hash, std::string poolKey, int difficult,int size);

#ifdef __cplusplus
}
#endif

#endif // ETENALGORITHM_H
