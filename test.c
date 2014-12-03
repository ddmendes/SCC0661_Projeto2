#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "huffman.h"

int main (int argc, char * argv[]) {

	byte stream[] = "DAVI DIORIO MENDES";
	byte * output;
	byte * decomp;
	unsigned int output_length, decomp_length;
	huffmanCompressor h = newHuffmanCompressor();
	compress(h, stream, strlen(stream), &output, &output_length);
	
	huffman_dump(h);

	int i;
	for(i = 0; i<output_length; i++) {
		printf("%d ", output[i]);
	}

	printf("\n");
	
	decompress(h, output, output_length, &decomp, &decomp_length);
	for(i = 0; i < decomp_length; i++) {
		printf("%c", decomp[i]);
	}

	free(output);
	free(decomp);
	
	return 0;
}