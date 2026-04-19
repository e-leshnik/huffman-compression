#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../src/Compressor.h"
#include "../src/Decompressor.h"

std::string readFile(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    std::ostringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

void writeFile(const std::string& filename, const std::string& data) {
    std::ofstream out(filename, std::ios::binary);
    out << data;
}

void testFullCycle(const std::string& text, const std::string& inputFile, const std::string& compressedFile, const std::string& outputFile) {
    writeFile(inputFile, text);
    Compressor::compress(inputFile, compressedFile);
    Decompressor::decompress(compressedFile, outputFile);

    std::string result = readFile(outputFile);
    assert(result == text);
}

void testBinaryFullCycle(const std::vector<unsigned char>& bytes, const std::string& inputFile, const std::string& compressedFile, const std::string& outputFile) {
    std::ofstream out(inputFile, std::ios::binary);
    out.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
    out.close();

    Compressor::compress(inputFile, compressedFile);
    Decompressor::decompress(compressedFile, outputFile);

    std::ifstream in(outputFile, std::ios::binary);
    std::vector<unsigned char> result(bytes.size());
    in.read(reinterpret_cast<char*>(result.data()), static_cast<std::streamsize>(result.size()));

    assert(result == bytes);
}

int main() {
    testFullCycle("abracadabra", "data/test_input_1.txt", "data/test_output_1.bin", "data/test_result_1.txt");
    testFullCycle("aaaaaaa", "data/test_input_2.txt", "data/test_output_2.bin", "data/test_result_2.txt");
    testFullCycle("abababab", "data/test_input_3.txt", "data/test_output_3.bin", "data/test_result_3.txt");
    testFullCycle("text with spaces", "data/test_input_4.txt", "data/test_output_4.bin", "data/test_result_4.txt");
    testFullCycle("line1\nline2\nline3", "data/test_input_5.txt", "data/test_output_5.bin", "data/test_result_5.txt");
    testFullCycle("12345!!!???...,,,000", "data/test_input_6.txt", "data/test_output_6.bin", "data/test_result_6.txt");
    testFullCycle(std::string(5000, 'a') + "bcdefghijklmnopqrstuvwxyz", "data/test_input_7.txt", "data/test_output_7.bin", "data/test_result_7.txt");
    testBinaryFullCycle({0x00, 0x01, 0x02, 0x03, 0xff, 0x80, 0x7f, 0x00, 0x55, 0xaa}, "data/test_input_8.bin", "data/test_output_8.bin", "data/test_result_8.bin");
    testBinaryFullCycle(
        []{
            std::vector<unsigned char> bytes;
            for (int i = 0; i < 256; i++) {
                bytes.push_back(static_cast<unsigned char>(i));
            }
            return bytes;
        }(),
        "data/test_input_9.bin", "data/test_output_9.bin", "data/test_result_9.bin"
    );

    std::cout << "Integration tests passed\n";
    return 0;
}