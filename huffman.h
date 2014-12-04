

typedef struct huffman_struct * HuffmanCompressor;
typedef struct persistent_huffman * PersistentHuffman;
typedef char byte;

struct occur {
    byte value;
    unsigned int occur;
};

struct persistent_huffman {
    struct occur ** symbol_occur;
    unsigned int symbol_count;
    byte * cstream;
    unsigned int cstream_length;
    int last_bit;
};

HuffmanCompressor newHuffmanCompressor();
void hcompress(HuffmanCompressor h, byte toCompress[], unsigned int inputLength, PersistentHuffman * persistentHuffman);
void hdecompress(HuffmanCompressor h, PersistentHuffman persistentHuffman, byte ** decompressedByteStream, unsigned int * byteStreamLength);
void huffman_dump(HuffmanCompressor h);
void persistentHuffmanFree(PersistentHuffman p);
