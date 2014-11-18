#include <stdlib.h>

#ifndef _HUFFMAN_H_
    #include <huffman.h>
#endif

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

void compress(huffmanCompressor h, byte * toCompress, int inputLength, byte ** compressedByteStream, unsigned int * byteStreamLength) {

}
