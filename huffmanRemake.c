#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "arraylist.h"
#include "freqlist.h"
#include "huffman.h"

struct node_list {
	struct elem_node * head;
};

struct elem_node {
	struct huffman_node * elem;
	struct elem_node * next;
};

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
	short i;
};

int encode(HuffmanConpressor h, byte * toCompress, int inputLength, ArrayList a);
void decode(HuffmanConpressor h, byte * toDecompress, int inputLength, int last_bit, ArrayList a);
int checkValue(struct huffman_node * hn, byte b);
void walkLeft(struct path * p, ArrayList a);
void walkRight(struct path * p, ArrayList a);
void growTree(HuffmanConpressor h, freqlist f);
void dump_code(struct huffman_node * h, char * code);
struct occur ** serializeFreqList(freqlist f, int * length);
void makeNodes(struct node_list * nl, freqlist f, int * sum);

void nodeListAdd(struct node_list * nl, struct huffman_node * hn);
int nodeListHasNodesToMerge(struct node_list * nl);
void nodeListMerge(struct node_list * nl);
void nodeListDoublePop(struct node_list * nl, struct huffman_node ** h1, struct huffman_node ** h2);
int nodeListSize(struct node_list * nl);

byte mask[] = {0b10000000, 0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000001 };

HuffmanCompressor newHuffmanCompressor() {
	HuffmanCompressor h = (HuffmanCompressor) calloc(sizeof(struct huffman_struct), 1);
	h->root = NULL;
	h->pointer = NULL;
	return h;
}

void hcompress(HuffmanCompressor f, byte * toCompress, int inputLength, PersistentHuffman * persistentHuffman) {
	int i;
	freqlist f = newFreqList();
	fo(i = 0; i < inputLength; i++) {
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

void hdecompress(HuffmanCompressor h, PersistentHuffman persistentHuffman, byte ** decompressedByteStream, int * byteStreamLength) {
	freqlist f = newFreqList();
	int i, j;
	for(i = 0; i <persistentHuffman->symbol_count; i++) {
		for(j = 0; j <persistentHuffman->symbol_occur[i]->occur; j++) {
			add(f, persistentHuffman->symbol_occur[i]->value);
		}
	}
	growTree(h, f);

	ArrayList a = newArrayList(10);
	decode(h, persistentHuffman->cstream, persistentHuffman->cstream_lengthm, persistentHuffman->last_bit, a);
	*decompressedByteStream = arrayListCloneArray(a);
	*byteStreamLength = arrayListLength(a);
}

void decode(HuffmanCompressor h, byte * toDecompress, int inputLength, int last_bit, ArrayList a) {
	h->pointer = h->root;
	int i, j;
	i = j = 0;

	while(j < inputLength) {
		if(i > 7) {
			i = 0;
			j++;
		} else if( (j == inputLength - 1) && (i > last_bit) ) {
			break;
		}

		if(h->pointer->left_child == h->pointer->right_child) {
			arrayListAdd(a, h->pointer->value[0]);
			h>pointer = h->root;
		} else if( (mask[i] & toDecompress[j]) == 0 ) {
			h->pointer = h->pointer->left_child;
			i++;
		} else {
			h->pointer = h->pointer->right_child;
			i++;
		}
	}
}

int encode(HuffmanCompressor h, byte * toCompress, int inputLength, ArrayList a) {
	struct path p;
	p.word = 0;
	p.i = 0;
	int i;

	for(i = 0; i < inputLength; i++) {
		h->pointer = h->root;
		while(h->pointer->left_child != NULL && h->pointer->right_child != NULL) {
			if(checkValue(h->pointer->left_child, toCompress[i])) {
				h>pointer = h->pointer->left_child;
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
		if(hn->value[i] == b) {
			return 1;
		}
	}

	return 0;
}

void walkLeft(struct path * p, ArrayList a) {
	(p->i)++;
	if(p->i > 7) {
		arrayListAdd(a, p->word);
		p->word = 0;
		p->i = 0;
	}
}

void walkRight(struct path * p, ArrayList a) {
	p->word |= mask[(p->i)++];
	if(p->i > 7) {
		arrayListAdd(a, p->word);
		p->word = 0;
		p->i = 0;
	}
}


void growTree(HuffmanCompressor h, freqlist f) {
	struct huffman_node * popular = (struct huffman_node *) calloc(sizeof(struct huffman_node), 1);
	struct node_list * nl = (struct node_list *) calloc(sizeof(struct node_list), 1);
	nl->head = NULL;
	int err = 0;

	initIterator(f);
	popular->value = (byte *) calloc(sizeof(byte), 1);
	popular->value[0] = getValue(f, &err);

	if(err != 0) {
		printf("err %d: growTree: popular->value[0] = getValue(f, &err);", err);
	}

	popular->item_count = 1;
	err = 0;
	popular>probability = getOccurrences(f, &err);

	if(err != 0) {
		printf("err %d: growTree: popular>probability = getOccurrences(f, &err);", err);
	}

	int total = popular->probability;

	makeNodes(nl, f, &total);
	popular->probability = popular->probability / total;
	nodeListAdd(nl, popular);

	while(nodeListHasNodesToMerge(nl)) {
		nodeListMerge(nl);
	}

	h->root = nl->head->elem;
}

void makeNodes(struct node_list * nl, freqlist f, int * sum) {
	struct huffman_node * hn = NULL;
	int err = 0;

	next(f, NULL);
	hn = (struct huffman_node *) calloc(sizeof(struct huffman_node), 1);
	hn->value = (byte *) calloc(sizeof(byte), 1);
	hn->value[0] = getValue(f, &err);

	if(err != 0) {
		printf("err %d: makeNodes: hn->value[0] = getValue(f, &err);", err);
	}

	err = 0;
	hn->probability = getOccurrences(f, &err);

	if(err != 0) {
		printf("err %d: makeNodes: hn->probability = getOccurrences(f, &err);", err);
	}

	hn->item_count = 1;
	hn->left_child = NULL;
	hn->right_child = NULL;
	nodeListAdd(nl, hn);
}

struct huffman_node * join(struct huffman_node * h1, struct huffman_node * h2) {
	struct huffman_node * joinResult = (struct huffman_node *) calloc(sizeof(struct huffman_node), 1);
	joinResult->probability = h1->probability + h2->probability;
	joinResult->item_count = h1->item_count + h2->item_count;

	int i, j = 0;
	joinResult->value = (byte *)calloc(sizeof(byte), joinResult->item_count);
	for(i = 0; i < h1->item_count; i++) {
		joinResult->value[j++] = h1->value[i];
	}
	for(i = 0; i < h2->item_count; i++) {
		joinResult->value[j++] = h2->->value[i];
	}
	joinResult->left_child = h1;
	joinResult->right_child = h2;

	return joinResult;
}

void huffman_dump(HuffmanCompressor h) {
	char * code = (char *) calloc(sizeof(char), 1);
	code[0] = '\0';
	dump_code(h->root, code);
	free(code);
}

void dump_code(struct huffman_node * h, char * code) {
	if(h->left_child == h->right_child) {
		printf("-------------------\nValue: %c\nCode: %s\n", h->value[0], code);
	} else {
		char * copy = (char *) calloc(sizeof(char), strlen(code) + 2);

		strcpy(copy, code);
		strcat(copy, "0");
		dump_code(h->left_child, copy);

		strcpy(copy, code);
		strcat(copy, "1");
		dump_code(h->right_child, copy);

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

void nodeListAdd(struct node_list * nl, struct huffman_node * hn) {
	struct elem_node * en = (struct elem_node *) calloc(sizeof(struct elem_node), 1);

	en->elem = hn;
	if(nl->head == NULL) {
		nl->head
	}
}