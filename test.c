#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CWVFile.h"

int main (int argc, char * argv[]) {
	FILE * cwv = fopen("files\\test3.cwv", "wb");
	FILE * wav = fopen("files\\test3_PCM_Mono_CBR_8SS_8000Hz.wav", "rb");

	//FILE * cwv = fopen("files\\test1.cwv", "wb");
	//FILE * wav = fopen("files\\test1_PCM_Stereo_CBR_16SS_6000Hz.wav", "rb");
	CWVCompress(cwv, wav);
	fclose(cwv);
	fclose(wav);

	cwv = fopen("files\\test3.cwv", "rb");
	wav = fopen("files\\test3_decomp.wav", "wb");
	CWVDecompress(wav, cwv);
	fclose(cwv);
	fclose(wav);

	return 0;
}