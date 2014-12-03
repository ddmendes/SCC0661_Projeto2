

typedef struct huffman_struct * huffmanCompressor;
typedef char byte;

huffmanCompressor newHuffmanCompressor();
void compress(huffmanCompressor h, byte * toCompress, unsigned int inputLength, byte ** compressedByteStream, unsigned int * byteStreamLength);
void huffman_dump(huffmanCompressor h);
