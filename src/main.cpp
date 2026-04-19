#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "HuffmanTree.h"
#include "BitWriter.h"
#include "BitReader.h"

std::string readFile(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Cannot open input file");
    }

    std::ostringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

void writeFile(const std::string& filename, const std::string& data) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Cannot open output file");
    }

    out << data;
}

void writeCompressedFile(const std::string& filename, const std::map<unsigned char, int>& freq, const std::string& encoded) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Cannot open compressed file");
    }

    int uniqueCount = static_cast<int>(freq.size());
    out.write(reinterpret_cast<const char*>(&uniqueCount), sizeof(int));

    for (const auto& pair : freq) {
        unsigned char symbol = pair.first;
        int frequency = pair.second;

        out.write(reinterpret_cast<const char*>(&symbol), sizeof(unsigned char));
        out.write(reinterpret_cast<const char*>(&frequency), sizeof(int));
    }

    int bitLength = static_cast<int>(encoded.size());
    out.write(reinterpret_cast<const char*>(&bitLength), sizeof(int));

    BitWriter writer(out);
    for (char bit : encoded) {
        writer.writeBit(bit == '1');
    }
    writer.flush();

    out.close();
}

std::string readCompressedFile(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Cannot open compressed file");
    }

    int uniqueCount;
    in.read(reinterpret_cast<char*>(&uniqueCount), sizeof(int));

    std::map<unsigned char, int> freq;

    for (int i = 0; i < uniqueCount; i++) {
        unsigned char symbol;
        int frequency;

        in.read(reinterpret_cast<char*>(&symbol), sizeof(unsigned char));
        in.read(reinterpret_cast<char*>(&frequency), sizeof(int));

        freq[symbol] = frequency;
    }

    int bitLength;
    in.read(reinterpret_cast<char*>(&bitLength), sizeof(int));
    in.peek();

    Node* root = HuffmanTree::buildTree(freq);

    BitReader reader(in);

    std::string encoded;
    encoded.reserve(bitLength);

    bool bit;
    int count = 0;

    while (count < bitLength && reader.readBit(bit)) {
        encoded += (bit ? '1' : '0');
        count++;
    }

    std::string decoded = HuffmanTree::decodeText(encoded, root);

    HuffmanTree::deleteTree(root);
    in.close();

    return decoded;
}

int main() {
    try {
        std::string inputFile = "data/input.txt";
        std::string outputFile = "data/output.txt";
        std::string compressedFile = "data/compressed.bin";

        std::string text = readFile(inputFile);

        if (text.empty()) {
            std::cout << "Input file is empty.\n";
            return 0;
        }

    auto freq = HuffmanTree::buildFrequency(text);
    Node* root = HuffmanTree::buildTree(freq);

    std::unordered_map<unsigned char, std::string> codes;
    HuffmanTree::buildCodes(root, "", codes);

    std::cout << "Codes:\n";
    for (const auto& pair : codes) {
        std::cout << pair.first << " : " << pair.second << "\n";
    }

    std::string encoded = HuffmanTree::encodeText(text, codes);
    std::cout << "\nEncoded length in bits: " << encoded.size() << "\n";

    writeCompressedFile(compressedFile, freq, encoded);

    std::cout << "Compressed data written to " << compressedFile << "\n";

    std::string decoded = readCompressedFile(compressedFile);
    writeFile(outputFile, decoded);
    std::cout << "\nDecoded text was written to\n" << outputFile << "\n";

    if (text == decoded) {
        std::cout << "SUCCESS: decoded matches input\n";
    } else {
        std::cout << "ERROR: mismatch\n";
    }

    HuffmanTree::deleteTree(root);
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }

    return 0;
}