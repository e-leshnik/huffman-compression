#include "BitWriter.h"
#include <stdexcept>

BitWriter::BitWriter(const std::string& filename) : buffer(0), bitCount(0) {
    out.open(filename, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Cannot open output file");
    }
}

void BitWriter::writeBit(bool bit) {
    buffer <<= 1;

    if (bit) {
        buffer |= 1;
    }

    bitCount++;

    if (bitCount == 8) {
        out.put(buffer);
        buffer = 0;
        bitCount = 0;
    }
}

void BitWriter::flush() {
    if (bitCount > 0) {
        buffer <<= (8 - bitCount);
        out.put(buffer);
        buffer = 0;
        bitCount = 0;
    }
}

void BitWriter::close() {
    flush();
    out.close();
}