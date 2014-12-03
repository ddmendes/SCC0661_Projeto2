#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "huffman.h"

void toFile(PersistentHuffman p, char * fname);
void fromFile(PersistentHuffman p, char * fname);

int main (int argc, char * argv[]) {

	byte stream[] = "DAVI DIORIO MENDES";
	HuffmanCompressor a, b;
	PersistentHuffman p;
	byte * output;
	unsigned int length;

	a = newHuffmanCompressor();
	compress(a, stream, strlen(stream), &p);

	toFile(p, "huf.bin");
	persistentHuffmanFree(p);
	p = (PersistentHuffman) calloc(sizeof(struct persistent_huffman), 1);
	fromFile(p, "huf.bin");

	b = newHuffmanCompressor();
	decompress(b, p, &output, &length);

	int i;
	for(i = 0; i < length; i++) {
		printf("%c", output[i]);
	}
	
	persistentHuffmanFree(p);
	return 0;
}

void toFile(PersistentHuffman p, char * fname) {
	FILE * f = fopen(fname, "wb");
	int i;
	fwrite(&(p->symbol_count), sizeof(unsigned int), 1, f);
	for(i = 0; i < p->symbol_count; i++) {
		fwrite(p->symbol_occur[i], sizeof(struct occur), 1, f);
	}
	fwrite(&(p->cstream_length), sizeof(unsigned int), 1, f);
	fwrite(p->cstream, sizeof(byte), p->cstream_length, f);
	fwrite(&(p->last_bit), sizeof(int), 1, f);
	fflush(f);
	fclose(f);
}

void fromFile(PersistentHuffman p, char * fname) {
	FILE *f = fopen(fname, "rb");
	int i;
	fread(&(p->symbol_count), sizeof(unsigned int), 1, f);
	p->symbol_occur = (struct occur **) calloc(sizeof(struct occur *), p->symbol_count);
	for(i = 0; i < p->symbol_count; i++) {
		p->symbol_occur[i] = (struct occur *) calloc(sizeof(struct occur), 1);
		fread(p->symbol_occur[i], sizeof(struct occur), 1, f);
	}
	fread(&(p->cstream_length), sizeof(unsigned int), 1, f);
	p->cstream = (byte *) calloc(sizeof(byte), p->cstream_length);
	fread(p->cstream, sizeof(byte), p->cstream_length, f);
	fread(&(p->last_bit) , sizeof(int), 1, f);
	fclose(f);
}