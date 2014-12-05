#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CWVFile.h"

void main (int argc, char * argv[]) {

	FILE *wav, *cwv;

	if(strcmp("-c", argv[1]) == 0) {
		wav = fopen(argv[2], "rb");
		cwv = fopen(argv[3], "wb");
		CWVCompress(cwv, wav);
		fclose(cwv);
		fclose(wav);
	} else if (strcmp("-d", argv[1]) == 0) {
		cwv = fopen(argv[2], "rb");
		wav = fopen(argv[3], "wb");
		CWVDecompress(wav, cwv);
		fclose(cwv);
		fclose(wav);
	} else {
		printf("WAVE File Compressor.\n");
		printf("USAGE: $ cwv [-c|-d] [input file] [output file]\n");
		printf("\t-c\tCompress.\n");
		printf("\t-d\tDecompress.\n");
		printf("\n");
	}

}