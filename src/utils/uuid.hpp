#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

inline std::string generate_uuidV4()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);

    uint32_t data[4];
    for (int i = 0; i < 4; ++i)
    {
        data[i] = dis(gen);
    }

    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    // 8 dígitos
    ss << std::setw(8) << (data[0]);
    ss << "-";

    // 4 dígitos
    ss << std::setw(4) << ((data[1] >> 16) & 0xFFFF);
    ss << "-";

    // 4 dígitos (versão 4 UUID)
    ss << std::setw(4) << (((data[1] >> 0) & 0x0FFF) | 0x4000);
    ss << "-";

    // 4 dígitos (variante 1: 0b10xxxxxx)
    ss << std::setw(4) << (((data[2] >> 16) & 0x3FFF) | 0x8000);
    ss << "-";

    // 12 dígitos
    ss << std::setw(4) << ((data[2]) & 0xFFFF);
    ss << std::setw(8) << (data[3]);

    return ss.str();
}
