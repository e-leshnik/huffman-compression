#include "Compressor.h"
#include "BitWriter.h"
#include "HuffmanTree.h"

#include <fstream>
#include <map>

void Compressor::compress(const std::string &inputFile,
                          const std::string &outputFile) {
  std::ifstream inputStream(inputFile, std::ios::binary);
  std::string text((std::istreambuf_iterator<char>(inputStream)),
                   std::istreambuf_iterator<char>());
  inputStream.close();

  auto freq = HuffmanTree::buildFrequency(text);
  Node *root = HuffmanTree::buildTree(freq);
  std::unordered_map<unsigned char, std::string> codes;
  HuffmanTree::buildCodes(root, "", codes);
  std::string encoded = HuffmanTree::encodeText(text, codes);
  std::ofstream out(outputFile, std::ios::binary);

  int size = static_cast<int>(freq.size());
  out.write(reinterpret_cast<char *>(&size), sizeof(int));

  for (const auto &pair : freq) {
    out.write(reinterpret_cast<const char *>(&pair.first),
              sizeof(unsigned char));
    out.write(reinterpret_cast<const char *>(&pair.second), sizeof(int));
  }

  int bitLength = static_cast<int>(encoded.size());
  out.write(reinterpret_cast<char *>(&bitLength), sizeof(int));

  BitWriter writer(out);

  for (char bit : encoded) {
    writer.writeBit(bit == '1');
  }

  writer.flush();
  out.close();
  HuffmanTree::deleteTree(root);
}