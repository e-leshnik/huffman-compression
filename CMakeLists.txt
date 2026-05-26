cmake_minimum_required(VERSION 3.16)

project(huffman_archiver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_SANITIZERS "Enable address and undefined sanitizers" OFF)

if (ENABLE_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

# Main
add_executable(huffman_archiver
    src/main.cpp
    src/HuffmanTree.cpp
    src/BitWriter.cpp
    src/BitReader.cpp
    src/Compressor.cpp
    src/Decompressor.cpp
)

target_include_directories(huffman_archiver PRIVATE src)

# Unit tests for HuffmanTree
add_executable(test_huffman
    tests/test_huffman.cpp
    src/HuffmanTree.cpp
)

target_include_directories(test_huffman PRIVATE src)

# Unit tests for bit I/O
add_executable(test_bit_io
    tests/test_bit_io.cpp
    src/BitWriter.cpp
    src/BitReader.cpp
)

target_include_directories(test_bit_io PRIVATE src)

# Integration tests
add_executable(test_integration
    tests/test_integration.cpp
    src/HuffmanTree.cpp
    src/BitWriter.cpp
    src/BitReader.cpp
    src/Compressor.cpp
    src/Decompressor.cpp
)

target_include_directories(test_integration PRIVATE src)