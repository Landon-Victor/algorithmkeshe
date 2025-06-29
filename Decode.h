#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <array>
#include <cstring>
#include <algorithm>

class SHA256 {
private:
    static const uint32_t K[64];

    std::array<uint32_t, 8> H = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    inline uint32_t rightRotate(uint32_t value, unsigned int count);

    void processBlock(const uint8_t* block);

public:
    std::vector<uint8_t> compute(const std::vector<uint8_t>& message);

    void reset();
};

class PasswordLock {
private:
    std::vector<unsigned char> salt = {
        0xb2, 0x53, 0x22, 0x65, 0x7d, 0xdf, 0xb0, 0xfe,
        0x9c, 0xde, 0xde, 0xfe, 0xf3, 0x1d, 0xdc, 0x3e
    };

    std::string bytesToHex(const std::vector<uint8_t>& bytes);

public:
    std::string hashPassword(const std::string& password);

    bool verifyPassword(const std::string& inputPassword, const std::string& storedHash);
};