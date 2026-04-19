#include "Decompressor.h"
#include "HuffmanTree.h"
#include "BitReader.h"

#include <fstream>
#include <map>

void Decompressor::decompress(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile, std::ios::binary);

    int size;
    in.read(reinterpret_cast<char*>(&size), sizeof(int));

    std::map<unsigned char, int> freq;

    for (int i = 0; i < size; i++) {
        unsigned char symbol;
        int count;

        in.read(reinterpret_cast<char*>(&symbol), sizeof(unsigned char));
        in.read(reinterpret_cast<char*>(&count), sizeof(int));

        freq[symbol] = count;
    }

    int bitLength;
    in.read(reinterpret_cast<char*>(&bitLength), sizeof(int));

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

    std::ofstream out(outputFile, std::ios::binary);
    out << decoded;
    out.close();

    in.close();
}