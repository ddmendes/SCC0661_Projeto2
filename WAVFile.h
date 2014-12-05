
typedef struct wav_file * WAVFile;
typedef char byte;

struct wav_file {
	int ChunkID;
	int ChunkSize;
	int Format;
	int Subchunk1ID;
	int Subchunk1Size;
	short AudioFormat;
	short NumChannels;
	int SampleRate;
	int ByteRate;
	short BlockAlign;
	int Subchunk2ID;
	int Subchunk2Size;
	byte * data;
};

WAVFile newWAVFile(FILE * wfile);
void WAVFileWrite(FILE * wfile, WAVFile wav);
void WAVFile_dump(WAVFile wav);