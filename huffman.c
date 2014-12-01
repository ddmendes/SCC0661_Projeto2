#include <stdlib.h>
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

struct huffman_node {
    byte node_value;
    struct huffman_node * left_child;
    struct huffman_node * right_child;
};

struct huffman_struct {
    struct huffman_node * root;
    struct huffman_node * pointer;
};

huffmanCompressor newHuffmanCompressor() {
    huffmanCompressor h = (huffmanCompressor) calloc(sizeof(huffman_struct), 1);
    h->root = NULL;
    h->pointer = NULL;
    return h;
}

void compress(huffmanCompressor h, byte toCompress[], unsigned int inputLength, byte compressedByteStream[], unsigned int * byteStreamLength) {

}
