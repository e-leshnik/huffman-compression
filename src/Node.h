#ifndef NODE_H
#define NODE_H

struct Node {
  unsigned char symbol;
  int freq;
  Node *left;
  Node *right;

  // Конструктор для листа
  Node(unsigned char symbolValue, int frequency) {
    symbol = symbolValue;
    freq = frequency;
    left = nullptr;
    right = nullptr;
  }

  // Конструктор для внутреннего узла
  Node(Node *leftChild, Node *rightChild) {
    symbol = 0;
    freq = leftChild->freq + rightChild->freq;
    left = leftChild;
    right = rightChild;
  }
};

#endif