#include "caesar.h"
#include <cctype>

std::string caesarEncrypt(const std::string& text, int shift) {
    std::string encrypted = "";

    // Iterate through each character in the text
    for (char ch : text) {
        // Check if the character is a lowercase letter
        if (std::islower(ch)) {
            // Shift the character by the specified shift amount (cyclically)
            char encryptedChar = 'a' + (ch - 'a' + shift) % 26;
            encrypted += encryptedChar;
        }
        // Check if the character is an uppercase letter
        else if (std::isupper(ch)) {
            // Shift the character by the specified shift amount (cyclically)
            char encryptedChar = 'A' + (ch - 'A' + shift) % 26;
            encrypted += encryptedChar;
        }
        // If the character is not a letter, append it unchanged
        else {
            encrypted += ch;
        }
    }

    return encrypted;
}
