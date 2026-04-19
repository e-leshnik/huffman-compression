#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H

#include <string>

class Decompressor {
public:
    static void decompress(const std::string& inputFile, const std::string& outputFile);
};

#endif