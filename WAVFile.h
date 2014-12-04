
typedef struct wav_file * WAVFile;
typedef char byte;

struct wav_file {
	unsigned int ChunkID;
	unsigned int ChunkSize;
	unsigned int Format;
	unsigned int Subchunk1ID;
	unsigned int Subchunk1Size;
	short AudioFormat;
	short NumChannels;
	unsigned int SampleRate;
	unsigned int ByteRate;
	short BlockAlign;
	unsigned int Subchunk2ID;
	unsigned int Subchunk2Size;
	byte * data;
};

WAVFile newWAVFile(FILE * wfile);
void WAVFileWrite(FILE * wfile, WAVFile wav);
void WAVFile_dump(WAVFile wav);