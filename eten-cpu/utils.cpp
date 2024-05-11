#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

string removeZeros(string str)
{
    std::string out = "";
    for(int i =0; i < str.length();i++)
    {
        if(str[i] != "0")
        {
            out += str[i];
        }
    }
    return out;
}