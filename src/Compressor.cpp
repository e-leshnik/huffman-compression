#include "Compressor.h"
#include "HuffmanTree.h"
#include "BitWriter.h"

#include <fstream>
#include <map>

void Compressor::compress(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile, std::ios::binary);
    std::string text((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();

    auto freq = HuffmanTree::buildFrequency(text);
    Node* root = HuffmanTree::buildTree(freq);
    std::unordered_map<unsigned char, std::string> codes;
    HuffmanTree::buildCodes(root, "", codes);
    std::string encoded = HuffmanTree::encodeText(text, codes);
    std::ofstream out(outputFile, std::ios::binary);

    int size = freq.size();
    out.write(reinterpret_cast<char*>(&size), sizeof(int));

    for (const auto& pair : freq) {
        out.write(reinterpret_cast<const char*>(&pair.first), sizeof(unsigned char));
        out.write(reinterpret_cast<const char*>(&pair.second), sizeof(int));
    }

    int bitLength = encoded.size();
    out.write(reinterpret_cast<char*>(&bitLength), sizeof(int));

    BitWriter writer(out);

    for (char bit : encoded) {
        writer.writeBit(bit == '1');
    }

    writer.flush();
    out.close();
}