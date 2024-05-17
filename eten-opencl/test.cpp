#include "base64.h"
#include "caesar.h"

int main()
{
    std::string test = "testowy tekst";
    test = encode(test);
    std::cout << test;
}