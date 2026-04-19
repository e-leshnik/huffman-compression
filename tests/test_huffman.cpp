#include <cassert>
#include <iostream>
#include <map>
#include <unordered_map>
#include <string>

#include "../src/HuffmanTree.h"

void testBuildFrequency() {
    std::string text = "aaabbc";
    auto freq = HuffmanTree::buildFrequency(text);

    assert(freq['a'] == 3);
    assert(freq['b'] == 2);
    assert(freq['c'] == 1);
}

void testEncodeDecodeSimple() {
    std::string text = "abracadabra";
    auto freq = HuffmanTree::buildFrequency(text);
    Node* root = HuffmanTree::buildTree(freq);
    std::unordered_map<unsigned char, std::string> codes;
    HuffmanTree::buildCodes(root, "", codes);
    std::string encoded = HuffmanTree::encodeText(text, codes);
    std::string decoded = HuffmanTree::decodeText(encoded, root);

    assert(decoded == text);

    HuffmanTree::deleteTree(root);
}

void testSingleCharacter() {
    std::string text = "aaaaaaa";
    auto freq = HuffmanTree::buildFrequency(text);
    Node* root = HuffmanTree::buildTree(freq);
    std::unordered_map<unsigned char, std::string> codes;
    HuffmanTree::buildCodes(root, "", codes);
    std::string encoded = HuffmanTree::encodeText(text, codes);
    std::string decoded = HuffmanTree::decodeText(encoded, root);

    assert(decoded == text);

    HuffmanTree::deleteTree(root);
}

void testTwoCharactersWithEqualFrequency() {
    std::string text = "abababab";
    auto freq = HuffmanTree::buildFrequency(text);
    Node* root = HuffmanTree::buildTree(freq);
    std::unordered_map<unsigned char, std::string> codes;
    HuffmanTree::buildCodes(root, "", codes);
    std::string encoded = HuffmanTree::encodeText(text, codes);
    std::string decoded = HuffmanTree::decodeText(encoded, root);

    assert(decoded == text);
    assert(codes.size() == 2);

    HuffmanTree::deleteTree(root);
}

void testAllUniqueCharacters() {
    std::string text = "abcdefg";
    auto freq = HuffmanTree::buildFrequency(text);
    Node* root = HuffmanTree::buildTree(freq);
    std::unordered_map<unsigned char, std::string> codes;
    HuffmanTree::buildCodes(root, "", codes);
    std::string encoded = HuffmanTree::encodeText(text, codes);
    std::string decoded = HuffmanTree::decodeText(encoded, root);

    assert(decoded == text);

    HuffmanTree::deleteTree(root);
}

void testWithSpaces() {
    std::string text = "hello world";
    auto freq = HuffmanTree::buildFrequency(text);
    Node* root = HuffmanTree::buildTree(freq);
    std::unordered_map<unsigned char, std::string> codes;
    HuffmanTree::buildCodes(root, "", codes);
    std::string encoded = HuffmanTree::encodeText(text, codes);
    std::string decoded = HuffmanTree::decodeText(encoded, root);

    assert(decoded == text);

    HuffmanTree::deleteTree(root);
}

void testWithNewLines() {
    std::string text = "hello\nworld\nabc";
    auto freq = HuffmanTree::buildFrequency(text);
    Node* root = HuffmanTree::buildTree(freq);
    std::unordered_map<unsigned char, std::string> codes;
    HuffmanTree::buildCodes(root, "", codes);
    std::string encoded = HuffmanTree::encodeText(text, codes);
    std::string decoded = HuffmanTree::decodeText(encoded, root);

    assert(decoded == text);

    HuffmanTree::deleteTree(root);
}

void testWithTabsAndNewLines() {
    std::string text = "hello\tworld\nabc\n\txyz";
    auto freq = HuffmanTree::buildFrequency(text);
    Node* root = HuffmanTree::buildTree(freq);
    std::unordered_map<unsigned char, std::string> codes;
    HuffmanTree::buildCodes(root, "", codes);
    std::string encoded = HuffmanTree::encodeText(text, codes);
    std::string decoded = HuffmanTree::decodeText(encoded, root);

    assert(decoded == text);

    HuffmanTree::deleteTree(root);
}

void testPunctuationAndDigits() {
    std::string text = "12345!!!???...,,,000";
    auto freq = HuffmanTree::buildFrequency(text);
    Node* root = HuffmanTree::buildTree(freq);
    std::unordered_map<unsigned char, std::string> codes;
    HuffmanTree::buildCodes(root, "", codes);
    std::string encoded = HuffmanTree::encodeText(text, codes);
    std::string decoded = HuffmanTree::decodeText(encoded, root);

    assert(decoded == text);

    HuffmanTree::deleteTree(root);
}

void testHighlySkewedDistribution() {
    std::string text(1000, 'a');
    text += "bcdef";
    auto freq = HuffmanTree::buildFrequency(text);
    Node* root = HuffmanTree::buildTree(freq);
    std::unordered_map<unsigned char, std::string> codes;
    HuffmanTree::buildCodes(root, "", codes);
    std::string encoded = HuffmanTree::encodeText(text, codes);
    std::string decoded = HuffmanTree::decodeText(encoded, root);

    assert(decoded == text);

    HuffmanTree::deleteTree(root);
}

void testBinaryLikeString() {
    std::string text;
    text.push_back('\0');
    text.push_back('\1');
    text.push_back('\2');
    text.push_back('\0');
    text.push_back('\xff');
    text.push_back('\n');
    text.push_back('\t');
    auto freq = HuffmanTree::buildFrequency(text);
    Node* root = HuffmanTree::buildTree(freq);
    std::unordered_map<unsigned char, std::string> codes;
    HuffmanTree::buildCodes(root, "", codes);
    std::string encoded = HuffmanTree::encodeText(text, codes);
    std::string decoded = HuffmanTree::decodeText(encoded, root);

    assert(decoded == text);

    HuffmanTree::deleteTree(root);
}

int main() {
    testBuildFrequency();
    testEncodeDecodeSimple();
    testSingleCharacter();
    testTwoCharactersWithEqualFrequency();
    testAllUniqueCharacters();
    testWithSpaces();
    testWithNewLines();
    testWithTabsAndNewLines();
    testPunctuationAndDigits();
    testHighlySkewedDistribution();
    testBinaryLikeString();

    std::cout << "All tests passed\n";
    return 0;
}