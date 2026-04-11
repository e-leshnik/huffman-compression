#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include "Node.h"
#include <unordered_map>
#include <string>

class HuffmanTree {
public:
    static std::unordered_map<unsigned char, int> buildFrequency(const std::string& text); // Подсчет частот символов
    static Node* buildTree(const std::unordered_map<unsigned char, int>&freq); // Построение дерева
    static void buildCodes(Node* root, const std::string& currentCode, std::unordered_map<unsigned char, std::string>& codes); // Построение кодов
    static std::string encodeText(const std::string& text, const std::unordered_map<unsigned char, std::string>& codes); // Возвращение закодированной строки
    static std::string decodeText(const std::string& encoded, Node* root); // Декодирование строки
    static void deleteTree(Node* root); // Освобождение памяти
};

#endif