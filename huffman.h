

typedef struct huffman_struct * HuffmanCompressor;
typedef struct persistent_huffman * PersistentHuffman;
typedef char byte;

/**
 * Estrutura de ocorrências.
 */
struct occur {
    byte value;
    int occur;
};

/**
 * Estrutura persistente de huffman.
 * Possui informações suficientes para decodificar a estrutura.
 */
struct persistent_huffman {
    struct occur ** symbol_occur;
    int symbol_count;
    byte * cstream;
    int cstream_length;
    int last_bit;
};

/**
 * Instancia um codificador de Huffman.
 */
HuffmanCompressor newHuffmanCompressor();

/**
 * Codifica um vetor de bytes em uma estrutura persistente de huffman.
 */
void hcompress(HuffmanCompressor h, byte * toCompress, int inputLength, PersistentHuffman * persistentHuffman);

/**
 * Decodifica uma estrutura persistente de huffman em um vetor de bytes.
 */
void hdecompress(HuffmanCompressor h, PersistentHuffman persistentHuffman, byte ** decompressedByteStream, int * byteStreamLength);

/**
 * Descreve a árvore de huffman gerada.
 */
void huffman_dump(HuffmanCompressor h);

/**
 * Libera a estrutura persistente de huffman.
 */
void persistentHuffmanFree(PersistentHuffman p);
