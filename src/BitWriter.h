#ifndef BIT_WRITER_H
#define BIT_WRITER_H

#include <fstream>

class BitWriter {
private:
    std::ofstream* out;
    unsigned char buffer;
    int bitCount;

public:
    BitWriter(std::ofstream& outputStream);
    void writeBit(bool bit);
    void flush();
};

#endif