#include "Decompressor.h"
#include "BitReader.h"
#include "HuffmanTree.h"

#include <fstream>
#include <map>

void Decompressor::decompress(const std::string &inputFile,
                              const std::string &outputFile) {
  std::ifstream in(inputFile, std::ios::binary);
  if (!in.is_open()) {
    throw std::runtime_error("Cannot open input file: " + inputFile);
  }

  int size;
  in.read(reinterpret_cast<char *>(&size), sizeof(int));

  std::map<unsigned char, int> freq;

  for (int i = 0; i < size; i++) {
    unsigned char symbol;
    int count;

    in.read(reinterpret_cast<char *>(&symbol), sizeof(unsigned char));
    in.read(reinterpret_cast<char *>(&count), sizeof(int));

    freq[symbol] = count;
  }

  int bitLength;
  in.read(reinterpret_cast<char *>(&bitLength), sizeof(int));

  Node *root = HuffmanTree::buildTree(freq);

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
  if (!out.is_open()) {
    throw std::runtime_error("Cannot open output file: " + outputFile);
  }
  out << decoded;
  out.close();

  in.close();
  HuffmanTree::deleteTree(root);
}