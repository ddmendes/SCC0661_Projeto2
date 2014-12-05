#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "freqlist.h"
#include "huffman.h"
#include "arraylist.h"

/**
 * Nó de sentinela da lista de nós de huffman.
 */
struct node_list {
    /**
     * Cabeça da lista.
     */
    struct elem_node * head;
};


/**
 * Nó da lista de nós de huffman.
 */
struct elem_node {
    /**
     * Ponteiro para nó de huffman.
     */
    struct huffman_node * elem;
    /**
     * Encadeamento da lista.
     */
    struct elem_node * next;
};

/**
 * Estrutura do nó de huffman.
 */
struct huffman_node {
    /**
     * Lista de valores contidos na subárvore.
     */
    byte * value;
    /**
     * Número de item contidos em vales.
     */
    int item_count;
    /**
     * Probabilidade de ocorrência do nó.
     */
    double probability;
    /**
     * Encadeamento esquerdo da árvore.
     */
    struct huffman_node * left_child;
    /**
     * Encadeamento direito da árvore.
     */
    struct huffman_node * right_child;
};

/**
 * Nó sentinela da árvore de huffman.
 */
struct huffman_struct {
    /**
     * Ponteiro para raiz da árvore.
     */
    struct huffman_node * root;
    /**
     * Ponteiro de navegação dentro da estrutura.
     */
    struct huffman_node * pointer;
};

/**
 * Estrutura de cálculo de caminhos na árvore.
 */
struct path {
    /**
     * Palavra atualmete sendo escrita.
     */
    byte word;
    /**
     * Bit atualmente sendo escrito.
    */
    short int i;
};

/**
 * Codifica um vetor de bytes atraves de huffman.
 * @param h Compressor de huffman em uso.
 * @param toCompress vetor a ser comprimido.
 * @param inputLength tamanho do vetor a ser comprimido.
 * @param a ArrayList com o resultado da compressão.
 * @return Endereço do ultimo bit escrito dentro do último byte.
 */
int encode(HuffmanCompressor h, byte * toCompress, int inputLength, ArrayList a);

/**
 * Decodifica um vetor de bytes atraves de huffman.
 * @param h Compressor de huffman em uso.
 * @param toDecompress vetor a ser descomprimido.
 * @param inputLength tamanho do vetor a ser descomprimido.
 * @param a ArrayList com o resultado da descompressão.
 */
void decode(HuffmanCompressor h, byte * toDecompress, int inputLength, int last_bit, ArrayList a);

/**
 * Verifica se um valor está em uma sub arvore.
 * @param hn Nó de huffman mais alto na sub árvore.
 * @param b byte a ser buscado.
 * @return 1 caso o byte foi encontrado na sub árvore, 0 caso contrário.
 */
int checkValue(struct huffman_node * hn, byte b);

/**
 * Escreve instrução de caminho à esquerda no byte de huffman sendo gerado.
 * @param p Estrutura de caminho em uso.
 * @param a ArrayList de saída do huffman.
 */
void walkLeft(struct path * p, ArrayList a);

/**
 * Escreve instrução de caminho à direita no byte de huffman sendo gerado.
 * @param p Estrutura de caminho em uso.
 * @param a ArrayList de saída do huffman.
 */
void walkRight(struct path * p, ArrayList a);

/**
 * Gera árvore de huffman a partir de uma lista de frequencias.
 * @param Compressor huffman sendo utilizado.
 * @param f lista de frequencias utilizada.
 */
void growTree(HuffmanCompressor h, freqlist f);

/**
 * Une dois nós de huffman em uma sub árvore.
 * @param h1 Nó a ser unido.
 * @param h2 Nó a ser unido.
 * @return sub árvore de huffman.
 */
struct huffman_node * join(struct huffman_node * h1, struct huffman_node * h2);

/**
 * Descreve árvore de huffman gerada.
 * @param h No atual de huffman.
 * @param code código atualmente gerado.
 */
void dump_code(struct huffman_node * h, char * code);

/**
 * Serializa lista de frequencias em um vetor de ocorrências.
 */
struct occur ** serializeFreqList(freqlist f, int * length);

/**
 * Gera nós da árvore de huffman.
 * @param nl Lista de nós de huffman gerados.
 * @param f Lista de frequencias em uso.
 * @param sum somatório de elementos.
 */
void makeNodes(struct node_list * nl, freqlist f, int * sum);

/**
 * adciona um novo elemento a lista de nós.
 * @param nl Lista de nós utilizados.
 * @param hn Nó de huffman a ser inserido na lista.
  */
void nodeListAdd(struct node_list * nl, struct huffman_node * hn);

/**
 * Verifica se há nós suficientes na lista para gerar uma sub arvore.
 * @param nl Lista de nós em uso.
 * @return 1 caso haja nos suficientes para uma operação de merge, 0 caso contrário.
 */
int nodeListHasNodesToMerge(struct node_list * nl);

/**
 * Une duas subárvores da lista de nós em uma e a reinsere na lista.
 * @param nl Lista de nós em uso.
 */
void nodeListMerge(struct node_list * nl);

/**
 * Remove duas sub árvores da cabeça da lista.
 * @param nl Lista de nós em uso.
 * @param h1 Ponteiro de retorno de nó de huffman extraído da lista.
 * @param h2 Ponteiro de retorno de nó de huffman extraído da lista.
 */
void nodeListDoublePop(struct node_list * nl, struct huffman_node ** h1, struct huffman_node ** h2);

/**
 * Recupera o tamanho da lista de nós;
 * @param nl Lista de nós em uso.
 * @return Número de nos na lista.
 */
int nodeListSize(struct node_list * nl);

byte mask[] = {0b10000000, 0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000001 };

HuffmanCompressor newHuffmanCompressor() {
    HuffmanCompressor h = (HuffmanCompressor) calloc(sizeof(struct huffman_struct), 1);
    h->root = NULL;
    return h;
}

void hcompress(HuffmanCompressor h, byte * toCompress, int inputLength, PersistentHuffman * persistentHuffman) {
    int i;
    freqlist f = newFreqList();
    for(i = 0; i < inputLength; i++) {
        add(f, toCompress[i]);
    }
    growTree(h, f);

    ArrayList a = newArrayList(10);
    int last_bit = encode(h, toCompress, inputLength, a);

    *persistentHuffman = (PersistentHuffman) calloc(sizeof(struct persistent_huffman), 1);
    (*persistentHuffman)->symbol_occur = serializeFreqList(f, &i);
    (*persistentHuffman)->symbol_count = i;
    (*persistentHuffman)->cstream = arrayListCloneArray(a);
    (*persistentHuffman)->cstream_length = arrayListLength(a);
    (*persistentHuffman)->last_bit = last_bit;
}

void hdecompress(HuffmanCompressor h, PersistentHuffman persistentHuffman, byte ** decompressedByteStream, int * byteStreamLength) {
    freqlist f = newFreqList();
    int i, j;
    for(i = 0; i < persistentHuffman->symbol_count; i++) {
        for(j = 0; j < persistentHuffman->symbol_occur[i]->occur; j++) {
            add(f, persistentHuffman->symbol_occur[i]->value);
        }
    }
    growTree(h, f);

    ArrayList a = newArrayList(10);
    decode(h, persistentHuffman->cstream, persistentHuffman->cstream_length, persistentHuffman->last_bit, a);
    *decompressedByteStream = arrayListCloneArray(a);
    *byteStreamLength = arrayListLength(a);
}

void decode(HuffmanCompressor h, byte * toDecompress, int inputLength, int last_bit, ArrayList a) {
    h->pointer = h->root;
    int i, j;
    i = j = 0;

    while(j < inputLength) {
        if(i > 7) {
            i = 0;
            j++;
        } else if( (j == inputLength - 1) && (i > last_bit) ) {
            break;
        }

        if(h->pointer->left_child == h->pointer->right_child) {
            arrayListAdd(a, h->pointer->value[0]);
            h->pointer = h->root;
        } else if( (mask[i] & toDecompress[j]) == 0 ) {
            h->pointer = h->pointer->left_child;
            i++;
        } else {
            h->pointer = h->pointer->right_child;
            i++;
        }
    }
}

int encode(HuffmanCompressor h, byte * toCompress, int inputLength, ArrayList a) {
    struct path p;
    p.word = 0;
    p.i = 0;
    int i;

    for(i = 0; i < inputLength; i++) {
        h->pointer = h->root;
        while(h->pointer->left_child != NULL && h->pointer->right_child != NULL) {
            if(checkValue(h->pointer->left_child, toCompress[i])){
                h->pointer = h->pointer->left_child;
                walkLeft(&p, a);
            } else {
                h->pointer = h->pointer->right_child;
                walkRight(&p, a);
            }
        }
    }

    if(p.i > 0) {
        arrayListAdd(a, p.word);
        return p.i;
    } else {
        return 7;
    }
}

int checkValue(struct huffman_node * hn, byte b) {
    int i;
    for(i = 0; i < hn->item_count; i++) {
        if(hn->value[i] == b)
            return 1;
    }

    return 0;
}

void walkLeft(struct path * p, ArrayList a) {
    p->i++;
    if(p->i > 7) {
        arrayListAdd(a, p->word);
        p->word = p->i = 0;
    }
}

void walkRight(struct path * p, ArrayList a) {
    p->word |= mask[p->i++];
    if(p->i > 7) {
        arrayListAdd(a, p->word);
        p->word = p->i = 0;
    }
}

void growTree(HuffmanCompressor h, freqlist f) {
    struct huffman_node * popular = (struct huffman_node *) calloc(sizeof(struct huffman_node), 1);
    struct node_list * nl = (struct node_list *) calloc(sizeof(struct node_list), 1);
    nl->head = NULL;

    initIterator(f);
    popular->value = (byte *) calloc(sizeof(byte), 1);
    popular->value[0] = getValue(f, NULL);
    popular->item_count = 1;
    popular->probability = getOccurrences(f, NULL);
    int total = popular->probability;

    makeNodes(nl, f, &total);
    popular->probability = popular->probability / total;
    nodeListAdd(nl, popular);

    while(nodeListHasNodesToMerge(nl)) {
        nodeListMerge(nl);
    }

    h->root = nl->head->elem;
}

void makeNodes(struct node_list * nl, freqlist f, int * sum) {
    struct huffman_node * hn = NULL;

    next(f, NULL);
    hn = (struct huffman_node *) calloc(sizeof(struct huffman_node), 1);
    hn->value = (byte *) calloc(sizeof(byte), 1);
    hn->value[0] = getValue(f, NULL);
    hn->probability = getOccurrences(f, NULL);
    hn->item_count = 1;
    hn->left_child = NULL;
    hn->right_child = NULL;

    *sum += hn->probability;

    if(hasNext(f)) {
        makeNodes(nl, f, sum);
    }

    hn->probability = hn->probability / *sum;
    nodeListAdd(nl, hn);
}

struct huffman_node * join(struct huffman_node * h1, struct huffman_node * h2) {
    struct huffman_node * joinResult = (struct huffman_node *) calloc(sizeof(struct huffman_node), 1);
    joinResult->probability = h1->probability + h2->probability;
    joinResult->item_count = h1->item_count + h2->item_count;

    int i, j = 0;
    joinResult->value = (byte *) calloc(sizeof(byte), joinResult->item_count);
    for(i = 0; i < h1->item_count; i++)
        joinResult->value[j++] = h1->value[i];
    for(i = 0; i < h2->item_count; i++)
        joinResult->value[j++] = h2->value[i];
    joinResult->left_child = h1;
    joinResult->right_child = h2;

    return joinResult;
}

void huffman_dump(HuffmanCompressor h) {
    char * code = (char *) calloc(sizeof(char), 1);
    code[0] = '\0';
    dump_code(h->root, code);
    free(code);
}

void dump_code(struct huffman_node * h, char * code) {
    if(h->left_child == NULL && h->right_child == NULL) {
        printf("-------------------\nValue: %c\nCode: %s\n", h->value[0], code);
    } else {
        char * copy = (char *) calloc(sizeof(char), strlen(code) + 2);

        strcpy(copy, code);
        strcat(copy, "0");
        dump_code(h->left_child, copy);

        strcpy(copy, code);
        strcat(copy, "1");
        dump_code(h->right_child, strcat(code, "1"));

        free(copy);
    }
}

struct occur ** serializeFreqList(freqlist f, int * length) {
    struct occur ** occurrences = (struct occur **) calloc(sizeof(struct occur *), *length = flistLength(f));
    int i = 0;

    for(initIterator(f); !endOfList(f); next(f, NULL)) {
        occurrences[i] = (struct occur *) calloc(sizeof(struct occur), 1);
        occurrences[i]->value = getValue(f, NULL);
        occurrences[i]->occur = getOccurrences(f, NULL);
        i++;
    }

    return occurrences;
}

void persistentHuffmanFree(PersistentHuffman p) {
    free(p->symbol_occur);
    free(p->cstream);
    free(p);
}

void nodeListAdd(struct node_list * nl, struct huffman_node * hn) {
    struct elem_node * en = (struct elem_node *) calloc(sizeof(struct elem_node), 1);
    
    en->elem = hn;
    if(nl->head == NULL) {
        nl->head = en;
        en->next = NULL;
    } else {
        struct elem_node * aux = nl->head;
        struct elem_node * back = NULL;
        while(aux != NULL && en->elem->probability > aux->elem->probability) {
            back = aux;
            aux = aux->next;
        }
        if(back == NULL) {
            nl->head = en;
            en->next = aux;
        } else {
            en->next = back->next;
            back->next = en;
        }
    }
}

int nodeListHasNodesToMerge(struct node_list * nl) {
    return (nl->head != NULL && nl->head->next != NULL);
}

void nodeListMerge(struct node_list * nl) {
    struct huffman_node * h1; 
    struct huffman_node * h2; 
    struct huffman_node * hr;
    nodeListDoublePop(nl, &h1, &h2);

    int i;
    for(i = 0; i < h1->item_count; i++) {
        if(h1->value[i] == 'I') {
            printf("h1\n");
        }
    }

    for(i = 0; i < h2->item_count; i++) {
        if(h2->value[i] == 'I') {
            printf("h2\n");
        }
    }

    hr = join(h1, h2);

    for(i = 0; i < hr->item_count; i++) {
        if(hr->value[i] == 'I') {
            printf("hr\n\n");
        }
    }

    nodeListAdd(nl, hr);
}

void nodeListDoublePop(struct node_list * nl, struct huffman_node ** h1, struct huffman_node ** h2) {
    if(nl->head != NULL && nl->head->next != NULL) {
        struct elem_node * aux;

        *h1 = nl->head->elem;
        *h2 = nl->head->next->elem;

        aux = nl->head;
        nl->head = nl->head->next->next;
        free(aux->next);
        free(aux);
    } else {
        *h1 = *h2 = NULL;
    }
}

int nodeListSize(struct node_list * nl) {
    int i;
    struct elem_node * en = nl->head;
    for(i = 0; en != NULL; i++) {
        en = en->next;
    }
    return i;
}