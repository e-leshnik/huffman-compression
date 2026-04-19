#include "Compressor.h"
#include "Decompressor.h"

#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Usage:\n";
        std::cout << " compress input.txt output.bin\n";
        std::cout << " decompress input.bin output.txt\n";
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "compress") {
        Compressor::compress(argv[2], argv[3]);
    } else if (mode == "decompress") {
        Decompressor::decompress(argv[2], argv[3]);
    } else {
        std::cout << "Unknown mode\n";
    }

    return 0;
}