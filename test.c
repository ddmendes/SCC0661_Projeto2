#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "huffman.h"

int main (int argc, char * argv[]) {

	byte stream[] = "DAVI DIORIO MENDES";
	byte * output;
	unsigned int output_length;
	huffmanCompressor h = newHuffmanCompressor();
	compress(h, stream, strlen(stream), &output, &output_length);
	
	huffman_dump(h);

	int i;
	for(i = 0; i<output_length; i++) {
		printf("%d ", output[i]);
	}
	
	return 0;
}