#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "freqlist.h"
#include "huffman.h"
#include "arraylist.h"

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
    byte * value;
    int item_count;
    double probability;
    struct huffman_node * left_child;
    struct huffman_node * right_child;
};

struct huffman_struct {
    struct huffman_node * root;
    struct huffman_node * pointer;
};

struct path {
    byte word;
    short int i;
};

int encode(HuffmanCompressor h, byte toCompress[], unsigned int inputLength, ArrayList a);
void decode(HuffmanCompressor h, byte * toDecompress, unsigned int inputLength, int last_bit, ArrayList a);
int checkValue(struct huffman_node * hn, byte b);
void walkLeft(struct path * p, ArrayList a);
void walkRight(struct path * p, ArrayList a);
void growTree(HuffmanCompressor h, freqlist f);
struct huffman_node * makeNodes(freqlist f, unsigned int * sum);
struct huffman_node ** sortNodes(struct huffman_node * n1, struct huffman_node * n2, struct huffman_node * n3);
struct huffman_node * join(struct huffman_node * leaf, struct huffman_node * subTree);
void dump_code(struct huffman_node * h, char * code);
struct occur ** serializeFreqList(freqlist f, int * length);

byte mask[] = {0b10000000, 0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000001 };

HuffmanCompressor newHuffmanCompressor() {
    HuffmanCompressor h = (HuffmanCompressor) calloc(sizeof(struct huffman_struct), 1);
    h->root = NULL;
    return h;
}

void compress(HuffmanCompressor h, byte toCompress[], unsigned int inputLength, PersistentHuffman * persistentHuffman) {
    int i;
    freqlist f = newFreqList();
    for(i = 0; i < inputLength; i++) {
        add(f, toCompress[i]);
    }
    growTree(h, f);

    ArrayList a = newArrayList(10);
    int last_bit = encode(h, toCompress, inputLength, a);

    *persistentHuffman = (PersistentHuffman) calloc(sizeof(struct persistent_huffman), 1);
    (*persistentHuffman)->symbol_occur = serializeFreqList(f, &i);
    (*persistentHuffman)->symbol_count = i;
    (*persistentHuffman)->cstream = arrayListCloneArray(a);
    (*persistentHuffman)->cstream_length = arrayListLength(a);
    (*persistentHuffman)->last_bit = last_bit;

}

void decompress(HuffmanCompressor h, PersistentHuffman persistentHuffman, byte ** decompressedByteStream, unsigned int * byteStreamLength) {
    freqlist f = newFreqList();
    int i, j;
    for(i = 0; i < persistentHuffman->symbol_count; i++) {
        for(j = 0; j < persistentHuffman->symbol_occur[i]->occur; j++) {
            add(f, persistentHuffman->symbol_occur[i]->value);
        }
    }
    growTree(h, f);

    ArrayList a = newArrayList(10);
    decode(h, persistentHuffman->cstream, persistentHuffman->cstream_length, persistentHuffman->last_bit, a);
    *decompressedByteStream = arrayListCloneArray(a);
    *byteStreamLength = arrayListLength(a);
}

void decode(HuffmanCompressor h, byte * toDecompress, unsigned int inputLength, int last_bit, ArrayList a) {
    h->pointer = h->root;
    int i, j;
    i = j = 0;

    while(j < inputLength) {
        if(i > 7) {
            i = 0;
            j++;
        } else if(j == inputLength - 1 && i > last_bit) {
            break;
        }

        if(h->pointer->left_child == h->pointer->right_child) {
            arrayListAdd(a, h->pointer->value[0]);
            h->pointer = h->root;
        } else if( (mask[i] & toDecompress[j]) == 0 ) {
            h->pointer = h->pointer->left_child;
            i++;
        } else {
            h->pointer = h->pointer->right_child;
            i++;
        }
    }
}

int encode(HuffmanCompressor h, byte * toCompress, unsigned int inputLength, ArrayList a) {
    struct path p;
    p.word = 0;
    p.i = 0;
    int i;

    for(i = 0; i < inputLength; i++) {
        h->pointer = h->root;
        while(h->pointer->left_child != NULL && h->pointer->right_child != NULL) {
            if(checkValue(h->pointer->left_child, toCompress[i])){
                h->pointer = h->pointer->left_child;
                walkLeft(&p, a);
            } else {
                h->pointer = h->pointer->right_child;
                walkRight(&p, a);
            }
        }
    }

    if(p.i > 0) {
        arrayListAdd(a, p.word);
        return p.i;
    } else {
        return 7;
    }
}

int checkValue(struct huffman_node * hn, byte b) {
    int i;
    for(i = 0; i < hn->item_count; i++) {
        if(hn->value[i] == b)
            return 1;
    }

    return 0;
}

void walkLeft(struct path * p, ArrayList a) {
    p->i++;
    if(p->i > 7) {
        arrayListAdd(a, p->word);
        p->word = p->i = 0;
    }
}

void walkRight(struct path * p, ArrayList a) {
    p->word |= mask[p->i++];
    if(p->i > 7) {
        arrayListAdd(a, p->word);
        p->word = p->i = 0;
    }
}

void growTree(HuffmanCompressor h, freqlist f) {
    struct huffman_node * popular = (struct huffman_node *) calloc(sizeof(struct huffman_node), 1);
    struct huffman_node * subTree;
    initIterator(f);
    popular->value = (byte *) calloc(sizeof(byte), 1);
    popular->value[0] = getValue(f, NULL);
    popular->item_count = 1;
    popular->probability = getOccurrences(f, NULL);
    unsigned int total = popular->probability;
    subTree = makeNodes(f, &total);
    h->root = join(popular, subTree);
}

struct huffman_node * makeNodes(freqlist f, unsigned int * sum) {
    struct huffman_node * hn = NULL;

    next(f, NULL);
    hn = (struct huffman_node *) calloc(sizeof(struct huffman_node), 1);
    hn->value = (byte *) calloc(sizeof(byte), 1);
    hn->value[0] = getValue(f, NULL);
    hn->probability = getOccurrences(f, NULL);
    hn->item_count = 1;
    hn->left_child = NULL;
    hn->right_child = NULL;

    *sum += hn->probability;

    if(hasNext(f)) {
        struct huffman_node ** aux = calloc(sizeof(struct huffman_node *), 2);
        aux[0] = hn;
        aux[1] = makeNodes(f, sum);
        hn->probability = hn->probability / *sum;
        hn = join(aux[0], aux[1]);
        free(aux);
    } else {
        hn->probability = hn->probability / *sum;
    }

    return hn;
}

struct huffman_node * join(struct huffman_node * leaf, struct huffman_node * subTree) {
    struct huffman_node * joinResult = (struct huffman_node *) calloc(sizeof(struct huffman_node), 1);
    joinResult->probability = leaf->probability + subTree->probability;
    joinResult->item_count = subTree->item_count + 1;

    //if(subTree->left_child == NULL && subTree->right_child == NULL) {
        int i;
        joinResult->value = (byte *) calloc(sizeof(byte), 1 + subTree->item_count);
        joinResult->value[0] = leaf->value[0];
        for(i = 0; i < subTree->item_count; i++)
            joinResult->value[i+1] = subTree->value[i];
        joinResult->left_child = leaf;
        joinResult->right_child = subTree;
    /*} else {
        int i, j;
        struct huffman_node ** nodes = sortNodes(leaf, subTree->left_child, subTree->right_child);
        struct huffman_node * aux =  (struct huffman_node *) calloc(sizeof(struct huffman_node), 1);

        aux->value = (byte *) calloc(sizeof(byte), nodes[0]->item_count + nodes[1]->item_count);
        for(i = j = 0; j < nodes[0]->item_count; i++, j++)
            aux->value[i] = nodes[0]->value[j];
        for(j = 0; j < nodes[1]->item_count; i++, j++)
            aux->value[i] = nodes[1]->value[j];

        aux->probability = nodes[0]->probability + nodes[1]->probability;
        aux->left_child = nodes[0];
        aux->right_child = nodes[1];

        joinResult->value = (byte *) calloc(sizeof(byte), nodes[2]->item_count + aux->item_count);
        for(i = j = 0; j < nodes[2]->item_count; i++, j++)
            joinResult->value[i] = nodes[2]->value[j];
        for(j = 0; j < aux->item_count; i++, j++)
            joinResult->value[i] = aux->value[j];

        joinResult->left_child = nodes[2];
        joinResult->right_child = aux;
    }*/

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

void huffman_dump(HuffmanCompressor h) {
    char * code = (char *) calloc(sizeof(char), 1);
    code[0] = '\0';
    dump_code(h->root, code);
    free(code);
}

void dump_code(struct huffman_node * h, char * code) {
    if(h->left_child == NULL && h->right_child == NULL) {
        printf("-------------------\nValue: %c\nCode: %s\n", h->value[0], code);
    } else {
        char * copy = (char *) calloc(sizeof(char), strlen(code) + 2);

        strcpy(copy, code);
        strcat(copy, "0");
        dump_code(h->left_child, copy);

        strcpy(copy, code);
        strcat(copy, "1");
        dump_code(h->right_child, strcat(code, "1"));

        free(copy);
    }
}

struct occur ** serializeFreqList(freqlist f, int * length) {
    struct occur ** occurrences = (struct occur **) calloc(sizeof(struct occur *), *length = flistLength(f));
    int i = 0;

    for(initIterator(f); !endOfList(f); next(f, NULL)) {
        occurrences[i] = (struct occur *) calloc(sizeof(struct occur), 1);
        occurrences[i]->value = getValue(f, NULL);
        occurrences[i]->occur = getOccurrences(f, NULL);
        i++;
    }

    return occurrences;
}

void persistentHuffmanFree(PersistentHuffman p) {
    free(p->symbol_occur);
    free(p->cstream);
    free(p);
}