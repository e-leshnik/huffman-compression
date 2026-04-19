#include "HuffmanTree.h"
#include <queue>
#include <vector>

std::map<unsigned char, int> HuffmanTree::buildFrequency(const std::string& text) {
    std::map<unsigned char, int> freq;

    for (unsigned char ch : text) {
        freq[ch]++;
    }

    return freq;
}

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

Node* HuffmanTree::buildTree(const std::map<unsigned char, int>& freq) {
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;

    for (const auto& pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }

    if (pq.empty()) {
        return nullptr;
    }

    while (pq.size() > 1) {
        Node* left = pq.top();
        pq.pop();

        Node* right = pq.top();
        pq.pop();

        Node* parent = new Node(left, right);
        pq.push(parent);
    }

    return pq.top();
    
}

void HuffmanTree::buildCodes(Node* root, const std::string& currentCode, std::unordered_map<unsigned char, std::string>& codes) {
    if (root == nullptr) {
        return;
    }

    // Если лист:
    if (root->left == nullptr && root->right == nullptr) {
        if (currentCode.empty()) {
            codes[root->symbol] = "0"; // когда только один символ
        } else {
            codes[root->symbol] = currentCode;
        }
        return;
    }

    buildCodes(root->left, currentCode + "0", codes);
    buildCodes(root->right, currentCode + "1", codes);
}

std::string HuffmanTree::encodeText(const std::string& text, const std::unordered_map<unsigned char, std::string>& codes) {
    std::string encoded;

    for (unsigned char ch : text) {
        encoded += codes.at(ch);
    }

    return encoded;
}

std::string HuffmanTree::decodeText(const std::string& encoded, Node* root) {
    if (root == nullptr) {
        return "";
    }
    // случай когда только один уникальный символ
    if (root->left == nullptr && root->right == nullptr) {
        return std::string(encoded.size(), root->symbol);
    }

    std::string decoded;
    Node* current = root;

    for (char bit : encoded) {
        if (bit == '0') {
            current = current->left;
        } else if (bit == '1') {
            current = current->right;
        }

        if (current->left == nullptr && current->right == nullptr) {
            decoded += current->symbol;
            current = root;
        }
    }

    return decoded;
}

void HuffmanTree::deleteTree(Node* root) {
    if (root == nullptr) {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}