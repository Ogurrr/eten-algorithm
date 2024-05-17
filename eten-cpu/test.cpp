#include "base64.h"
#include "caesar.h"

int main()
{
    std::string test = "testowy tekst";
    std::vector<std::string> testBinarySplit = splitBinary(test);
    std::cout << encode(test);
}