#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "HuffmanTree.h"

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

int main() {
    try {
        std::string inputFile = "data/input.txt";
        std::string outputFile = "data/output.txt";

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
        std::cout << pair.first << " : " << pair.second << std::endl;
    }

    std::string encoded = HuffmanTree::encodeText(text, codes);
    std::cout << "\nEncoded length in bits: " << encoded.size() << "\n";

    std::string decoded = HuffmanTree::decodeText(encoded, root);
    writeFile(outputFile, decoded);
    std::cout << "\nDecoded text was written to\n" << outputFile << "\n";

    HuffmanTree::deleteTree(root);
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }

    return 0;
}