#ifndef BIT_READER_H
#define BIT_READER_H

#include <fstream>

class BitReader {
private:
    std::ifstream* in;
    unsigned char buffer;
    int bitCount;

public:
    BitReader(std::ifstream& inputStream);
    bool readBit(bool& bit);
};

#endif