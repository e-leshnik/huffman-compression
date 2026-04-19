#include "BitReader.h"

BitReader::BitReader(std::ifstream& inputStream) : in(&inputStream), buffer(0), bitCount(0) {}

bool BitReader::readBit(bool& bit) {
    if (bitCount == 0) {
        if (!in->get(reinterpret_cast<char&>(buffer))) {
            return false; // конец файла
        }
        bitCount = 8;
    }

    bit = (buffer & 0b10000000) != 0;
    buffer <<= 1;
    bitCount--;

    return true;
}