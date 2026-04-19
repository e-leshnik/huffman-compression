#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../src/BitWriter.h"
#include "../src/BitReader.h"

std::string readBitsFromFile(const std::string& filename, int bitCount) {
    std::ifstream in(filename, std::ios::binary);
    BitReader reader(in);

    std::string bits;
    bits.reserve(bitCount);

    bool bit;
    int count = 0;
    while (count < bitCount && reader.readBit(bit)) {
        bits += (bit ? '1' : '0');
        count++;
    }

    in.close();
    return bits;
}

void writeBitsToFile(const std::string& filename, const std::string& bits) {
    std::ofstream out(filename, std::ios::binary);
    BitWriter writer(out);

    for (char bit : bits) {
        writer.writeBit(bit == '1');
    }

    writer.flush();
    out.close();
}

void testExactlyOneByte() {
    std::string bits = "10110010";
    std::string file = "data/test_bits_1.bin";

    writeBitsToFile(file, bits);
    std::string result = readBitsFromFile(file, static_cast<int>(bits.size()));

    assert(result == bits);
}

void testIncompleteLastByte() {
    std::string bits = "1011001";
    std::string file = "data/test_bits_2.bin";

    writeBitsToFile(file, bits);
    std::string result = readBitsFromFile(file, static_cast<int>(bits.size()));

    assert(result == bits);
}

void testSeveralBytes() {
    std::string bits = "101100100011011011001010";
    std::string file = "data/test_bits_3.bin";

    writeBitsToFile(file, bits);
    std::string result = readBitsFromFile(file, static_cast<int>(bits.size()));

    assert(result == bits);
}

void testAllZeros() {
    std::string bits = "0000000000000000";
    std::string file = "data/test_bits_4.bin";

    writeBitsToFile(file, bits);
    std::string result = readBitsFromFile(file, static_cast<int>(bits.size()));

    assert(result == bits);
}

void testAllOnes() {
    std::string bits = "1111111111111111";
    std::string file = "data/test_bits_5.bin";

    writeBitsToFile(file, bits);
    std::string result = readBitsFromFile(file, static_cast<int>(bits.size()));

    assert(result == bits);
}

void testAlternatingBits() {
    std::string bits = "010101010101010101010101";
    std::string file = "data/test_bits_1.bin";

    writeBitsToFile(file, bits);
    std::string result = readBitsFromFile(file, static_cast<int>(bits.size()));

    assert(result == bits);
}

void testSingleZeroBit() {
    std::string bits = "0";
    std::string file = "data/test_bits_1.bin";

    writeBitsToFile(file, bits);
    std::string result = readBitsFromFile(file, static_cast<int>(bits.size()));

    assert(result == bits);
}

void testSingleOneBit() {
    std::string bits = "1";
    std::string file = "data/test_bits_1.bin";

    writeBitsToFile(file, bits);
    std::string result = readBitsFromFile(file, static_cast<int>(bits.size()));

    assert(result == bits);
}

int main() {
    testExactlyOneByte();
    testIncompleteLastByte();
    testSeveralBytes();
    testAllZeros();
    testAllOnes();
    testAlternatingBits();
    testSingleZeroBit();
    testSingleOneBit();

    std::cout << "All bit I/O tests passed\n";
    return 0;
}