#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "WAVFile.h"

WAVFile newWAVFile(FILE * wfile) {
	WAVFile wav = (WAVFile) calloc(sizeof(struct wav_file), 1);

	if(wfile != NULL) {
		fseek(wfile, 0, SEEK_SET);
		fread(wav, sizeof(struct wav_file) - sizeof(byte *), 1, wfile);
		wav->data = (byte *) calloc(sizeof(byte), wav->Subchunk2Size);
		fread(wav->data, wav->Subchunk2Size, 1, wfile);
	} else {
		wav->ChunkID = 0x46464952;
		wav->Format  = 0x45564157;
		wav->Subchunk1ID = 0x20746d66;
		wav->Subchunk1Size = 16;
		wav->AudioFormat = 1;
		wav->Subchunk2ID = 0x61746164;
	}

	return wav;
}

void WAVFileWrite(FILE * wfile, WAVFile wav) {
	fwrite(wav, sizeof(struct wav_file) - sizeof(byte *), 1, wfile);
	fwrite(wav->data, sizeof(byte), wav->Subchunk2Size, wfile);
	fflush(wfile);
}

void WAVFile_dump(WAVFile wav) {
	printf("----------------------\n");
	printf("WAVE FILE DUMP\n");
	printf("ChunkID: %x\n", wav->ChunkID);
	printf("ChunkSize: %d\n", wav->ChunkSize);
	printf("Format: %x\n", wav->Format);
	printf("Subchunk1ID: %x\n", wav->Subchunk1ID);
	printf("Subchunk1Size: %d\n", wav->Subchunk1Size);
	printf("AudioFormat: %d\n", wav->AudioFormat);
	printf("NumChannels: %d\n", wav->NumChannels);
	printf("SampleRate: %d\n", wav->SampleRate);
	printf("ByteRate: %d\n", wav->ByteRate);
	printf("BlockAlign: %d\n", wav->BlockAlign);
	printf("Subchunk2ID: %x\n", wav->Subchunk2ID);
	printf("Subchunk2Size: %d\n", wav->Subchunk2Size);
	printf("----------------------\n");
}