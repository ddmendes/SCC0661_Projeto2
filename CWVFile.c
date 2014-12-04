#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CWVFile.h"
#include "WAVFile.h"
#include "huffman.h"

#define HEADER_SIZE 12

struct cwv_file {
	short NumChannels;
	int   SampleRate;
	int   ByteRate;
	short BlockAlign;
	PersistentHuffman PersHuffman;
};

void toFile(FILE * cwv, struct cwv_file c);
void fromFile(struct cwv_file * c, FILE * cwv);

void CWVCompress(FILE * cwv, FILE * wav) {
	WAVFile w;
	HuffmanCompressor h;
	struct cwv_file c;
	if(wav != NULL) {
		w = newWAVFile(wav);
		WAVFile_dump(w);
		h = newHuffmanCompressor();

		c.NumChannels = w->NumChannels;
		c.SampleRate = w->SampleRate;
		c.ByteRate = w->ByteRate;
		c.BlockAlign = w->BlockAlign;
		hcompress(h, w->data, w->Subchunk2Size, &(c.PersHuffman));

		toFile(cwv, c);
	}
}

void CWVDecompress(FILE * wav, FILE * cwv) {
	WAVFile w;
	HuffmanCompressor h;
	struct cwv_file c;
	if(cwv != NULL) {
		w = newWAVFile(NULL);
		h = newHuffmanCompressor();

		fromFile(&c, cwv);
		hdecompress(h, c.PersHuffman, &(w->data), &(w->Subchunk2Size));
		w->NumChannels = c.NumChannels;
		w->SampleRate = c.SampleRate;
		w->ByteRate = c.ByteRate;
		w->BlockAlign = c.BlockAlign;
		w->ChunkSize = 36 + w->Subchunk2Size;
		WAVFile_dump(w);
		WAVFileWrite(wav, w);
	}
}

void toFile(FILE * cwv, struct cwv_file c) {
	int i;

	fwrite(&(c), HEADER_SIZE, 1, cwv);
	fwrite(&(c.PersHuffman->symbol_count), sizeof(unsigned int), 1, cwv);
	for(i = 0; i < c.PersHuffman->symbol_count; i++) {
		fwrite(c.PersHuffman->symbol_occur[i], sizeof(struct occur), 1, cwv);
	}
	fwrite(&(c.PersHuffman->cstream_length), sizeof(unsigned int), 1, cwv);
	fwrite(c.PersHuffman->cstream, sizeof(byte), c.PersHuffman->cstream_length, cwv);
	fwrite(&(c.PersHuffman->last_bit), sizeof(int), 1, cwv);
	fflush(cwv);
}

void fromFile(struct cwv_file * c, FILE * cwv) {
	int i;

	fread(c, sizeof(struct cwv_file) - sizeof(PersistentHuffman), 1, cwv);
	fread(&(c->PersHuffman->symbol_count), sizeof(unsigned int), 1, cwv);
	c->PersHuffman->symbol_occur = (struct occur **) calloc(sizeof(struct occur *), c->PersHuffman->symbol_count);
	for(i = 0; i < c->PersHuffman->symbol_count; i++) {
		c->PersHuffman->symbol_occur[i] = (struct occur *) calloc(sizeof(struct occur), 1);
		fread(c->PersHuffman->symbol_occur[i], sizeof(struct occur), 1, cwv);
	}
	fread(&(c->PersHuffman->cstream_length), sizeof(unsigned int), 1, cwv);
	c->PersHuffman->cstream = (byte *) calloc(sizeof(byte), c->PersHuffman->cstream_length);
	fread(c->PersHuffman->cstream, sizeof(byte), c->PersHuffman->cstream_length, cwv);
	fread(&(c->PersHuffman->last_bit) , sizeof(int), 1, cwv);
}