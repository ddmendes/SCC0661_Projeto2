#include <stdio.h>
#include <stdlib.h>
#include "freqlist.h"
#include "huffman.h"

//Bit masks
#define BIT_1ST 0b00000001
#define BIT_2ND 0b00000010
#define BIT_3RD 0b00000100
#define BIT_4TH 0b00001000
#define BIT_5TH 0b00010000
#define BIT_6TH 0b00100000
#define BIT_7TH 0b01000000
#define BIT_8TH 0b10000000

void growTree(huffmanCompressor h, freqlist f);
struct huffman_node * makeNodes(freqlist f, unsigned int * sum);
struct huffman_node ** sortNodes(struct huffman_node * n1, struct huffman_node * n2, struct huffman_node * n3);
struct huffman_node * join(struct huffman_node * leaf, struct huffman_node * subTree);

struct huffman_node {
    byte value;
    double probability;
    struct huffman_node * left_child;
    struct huffman_node * right_child;
};

struct huffman_struct {
    struct huffman_node * root;
    struct huffman_node * pointer;
};

struct value_code_pair {
    byte value;
    char * code;
}

huffmanCompressor newHuffmanCompressor() {
    huffmanCompressor h = (huffmanCompressor) calloc(sizeof(struct huffman_struct), 1);
    h->root = NULL;
    h->pointer = NULL;
    return h;
}

void compress(huffmanCompressor h, byte toCompress[], unsigned int inputLength, byte compressedByteStream[], unsigned int * byteStreamLength) {
    int i;
    freqlist f = newFreqList();
    for(i = 0; i < inputLength; i++) {
        add(f, toCompress[i]);
    }
    growTree(h, f);
}

void growTree(huffmanCompressor h, freqlist f) {
    struct huffman_node * popular = (struct huffman_node *) calloc(sizeof(struct huffman_node), 1);
    struct huffman_node * subTree;
    initIterator(f);
    next(f, NULL);
    popular->value = getValue(f, NULL);
    popular->probability = getOccurrences(f, NULL);
    unsigned int total = popular->probability;
    subTree = makeNodes(f, &total);
    h->root = join(popular, subTree);
}

struct huffman_node * makeNodes(freqlist f, unsigned int * sum) {
    struct huffman_node * hn = NULL;

    next(f, NULL);
    hn = (struct huffman_node *) calloc(sizeof(struct huffman_node), 1);
    hn->value = getValue(f, NULL);
    hn->probability = getOccurrences(f, NULL);
    hn->left_child = NULL;
    hn->right_child = NULL;

    *sum += hn->probability;

    if(hasNext(f)) {
        struct huffman_node * aux = makeNodes(f, sum);
        hn->probability = hn->probability / *sum;
        hn = join(hn, aux);
    } else {
        hn->probability = hn->probability / *sum;
    }

    return hn;
}

struct huffman_node * join(struct huffman_node * leaf, struct huffman_node * subTree) {
    struct huffman_node * joinResult = (struct huffman_node *) calloc(sizeof(struct huffman_node), 1);
    joinResult->value = 0;
    joinResult->probability = leaf->probability + subTree->probability;

    if(subTree->left_child == NULL && subTree->right_child == NULL) {
        joinResult->left_child = leaf;
        joinResult->right_child = subTree;
    } else {
        struct huffman_node ** nodes = sortNodes(leaf, subTree->left_child, subTree->right_child);
        struct huffman_node * aux =  (struct huffman_node *) calloc(sizeof(struct huffman_node), 1);

        aux->value = 0;
        aux->probability = nodes[0]->probability + nodes[1]->probability;
        aux->left_child = nodes[0];
        aux->right_child = nodes[1];

        joinResult->left_child = nodes[2];
        joinResult->right_child = aux;
    }

    return joinResult;
}

struct huffman_node ** sortNodes(struct huffman_node * n1, struct huffman_node * n2, struct huffman_node * n3) {
    struct huffman_node ** nodes = (struct huffman_node **) calloc(sizeof(struct huffman_node *), 3);
    struct huffman_node * aux;

    nodes[0] = n1;
    nodes[1] = n2;
    nodes[2] = n3;

    if(nodes[1]->probability > nodes[2]->probability) {
        aux = nodes[1];
        nodes[1] = nodes[2];
        nodes[2] = aux;
    }

    if(nodes[0]->probability > nodes[2]->probability) {
        aux = nodes[0];
        nodes[0] = nodes[2];
        nodes[2] = aux;
    } else if(nodes[0]->probability > nodes[1]->probability) {
        aux = nodes[0];
        nodes[0] = nodes[1];
        nodes[1] = aux;
    }

    return nodes;
}

void huffman_dump(huffmanCompressor h) {
    
}