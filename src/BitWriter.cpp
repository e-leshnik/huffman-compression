#include "BitWriter.h"

BitWriter::BitWriter(std::ofstream &outputStream)
    : out(&outputStream) {}

void BitWriter::writeBit(bool bit) {
  buffer <<= 1;

  if (bit) {
    buffer |= 1;
  }

  bitCount++;

  if (bitCount == 8) {
    out->put(static_cast<char>(buffer));
    buffer = 0;
    bitCount = 0;
  }
}

void BitWriter::flush() {
  if (bitCount > 0) {
    buffer <<= (8 - bitCount);
    out->put(static_cast<char>(buffer));
    buffer = 0;
    bitCount = 0;
  }
}