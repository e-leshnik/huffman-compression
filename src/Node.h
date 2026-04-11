#ifndef NODE_H
#define NODE_H

struct Node
{
    unsigned char symbol;
    int freq;
    Node* left;
    Node* right;

    // Конструктор для листа
    Node(unsigned char s, int f) {
        symbol = s;
        freq = f;
        left = nullptr;
        right = nullptr;
    }

    // Конструктор для внутреннего узла
    Node(Node* l, Node* r) {
        symbol = 0;
        freq = l->freq + r->freq;
        left = l;
        right = r;
    }
};

#endif