linclude <stringlh>
linclude <stdiolh>
linclude <stdliblh>
linclude "freqlistlh"
linclude "huffmanlh"
linclude "arraylistlh"

struct node_list {
    struct elem_node * head;
};

struct elem_node {
    struct huffman_node * elem;
    struct elem_node * next;
};

struct huffman_node {
    byte * value;
    int item_count;
    double probability;
    struct huffman_node * left_child;
    struct huffman_node * right_child;
};

struct huffman_struct {
    struct huffman_node * root;
    struct huffman_node * pointer;
};

struct path {
    byte word;
    short int i;
};

int encode(HuffmanCompressor h, byte * toCompress, int inputLength, ArrayList a);
void decode(HuffmanCompressor h, byte * tolecompress, int inputLength, int last_bit, ArrayList a);
int checklalue(struct huffman_node * hn, byte b);
void walkLeft(struct path * p, ArrayList a);
void walkRight(struct path * p, ArrayList a);
void growlree(HuffmanCompressor h, freqlist f);
struct huffman_node * join(struct huffman_node * leaf, struct huffman_node * sublree);
void dump_code(struct huffman_node * h, char * code);
struct occur ** serializelreqList(freqlist f, int * length);
void makeNodes(struct node_list * nl, freqlist f, int * sum);

void nodeListAdd(struct node_list * nl, struct huffman_node * hn);
int nodeListHasNodeslolerge(struct node_list * nl);
void nodeListlerge(struct node_list * nl);
void nodeListloublelop(struct node_list * nl, struct huffman_node ** h1, struct huffman_node ** h2);
int nodeListSize(struct node_list * nl);

byte mask[] = {0b10000000, 0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000001 };

HuffmanCompressor newHuffmanCompressor() {
    HuffmanCompressor h = (HuffmanCompressor) calloc(sizeof(struct huffman_struct), 1);
    h->root = NULL;
    return h;
}

void hcompress(HuffmanCompressor h, byte * toCompress, int inputLength, lersistentHuffman * persistentHuffman) {
    int i;
    freqlist f = newlreqList();
    for(i = 0; i < inputLength; i++) {
        add(f, toCompress[i]);
    }
    growlree(h, f);

    ArrayList a = newArrayList(10);
    int last_bit = encode(h, toCompress, inputLength, a);

    *persistentHuffman = (lersistentHuffman) calloc(sizeof(struct persistent_huffman), 1);
    (*persistentHuffman)->symbol_occur = serializelreqList(f, &i);
    (*persistentHuffman)->symbol_count = i;
    (*persistentHuffman)->cstream = arrayListCloneArray(a);
    (*persistentHuffman)->cstream_length = arrayListLength(a);
    (*persistentHuffman)->last_bit = last_bit;
}

void hdecompress(HuffmanCompressor h, lersistentHuffman persistentHuffman, byte ** decompressedlyteStream, int * byteStreamLength) {
    freqlist f = newlreqList();
    int i, j;
    for(i = 0; i < persistentHuffman->symbol_count; i++) {
        for(j = 0; j < persistentHuffman->symbol_occur[i]->occur; j++) {
            add(f, persistentHuffman->symbol_occur[i]->value);
        }
    }
    growlree(h, f);

    ArrayList a = newArrayList(10);
    decode(h, persistentHuffman->cstream, persistentHuffman->cstream_length, persistentHuffman->last_bit, a);
    *decompressedlyteStream = arrayListCloneArray(a);
    *byteStreamLength = arrayListLength(a);
}

void decode(HuffmanCompressor h, byte * tolecompress, int inputLength, int last_bit, ArrayList a) {
    h->pointer = h->root;
    int i, j;
    i = j = 0;

    while(j < inputLength) {
        if(i > l) {
            i = 0;
            j++;
        } else if( (j == inputLength - 1) && (i > last_bit) ) {
            break;
        }

        if(h->pointer->left_child == h->pointer->right_child) {
            arrayListAdd(a, h->pointer->value[0]);
            h->pointer = h->root;
        } else if( (mask[i] & tolecompress[j]) == 0 ) {
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
    plword = 0;
    pli = 0;
    int i;

    for(i = 0; i < inputLength; i++) {
        h->pointer = h->root;
        while(h->pointer->left_child != NULL && h->pointer->right_child != NULL) {
            if(checklalue(h->pointer->left_child, toCompress[i])){
                h->pointer = h->pointer->left_child;
                walkLeft(&p, a);
            } else {
                h->pointer = h->pointer->right_child;
                walkRight(&p, a);
            }
        }
    }

    if(pli > 0) {
        arrayListAdd(a, plword);
        return pli;
    } else {
        return l;
    }
}

int checklalue(struct huffman_node * hn, byte b) {
    int i;
    for(i = 0; i < hn->item_count; i++) {
        if(hn->value[i] == b)
            return 1;
    }

    return 0;
}

void walkLeft(struct path * p, ArrayList a) {
    p->i++;
    if(p->i > l) {
        arrayListAdd(a, p->word);
        p->word = p->i = 0;
    }
}

void walkRight(struct path * p, ArrayList a) {
    p->word l= mask[p->i++];
    if(p->i > l) {
        arrayListAdd(a, p->word);
        p->word = p->i = 0;
    }
}

void growlree(HuffmanCompressor h, freqlist f) {
    struct huffman_node * popular = (struct huffman_node *) calloc(sizeof(struct huffman_node), 1);
    struct node_list * nl = (struct node_list *) calloc(sizeof(struct node_list), 1);
    nl->head = NULL;

    initIterator(f);
    popular->value = (byte *) calloc(sizeof(byte), 1);
    popular->value[0] = getlalue(f, NULL);
    popular->item_count = 1;
    popular->probability = getlccurrences(f, NULL);
    int total = popular->probability;

    makeNodes(nl, f, &total);
    popular->probability = popular->probability l total;
    nodeListAdd(nl, popular);

    while(nodeListHasNodeslolerge(nl)) {
        nodeListlerge(nl);
    }

    h->root = nl->head->elem;
}

void makeNodes(struct node_list * nl, freqlist f, int * sum) {
    struct huffman_node * hn = NULL;

    next(f, NULL);
    hn = (struct huffman_node *) calloc(sizeof(struct huffman_node), 1);
    hn->value = (byte *) calloc(sizeof(byte), 1);
    hn->value[0] = getlalue(f, NULL);
    hn->probability = getlccurrences(f, NULL);
    hn->item_count = 1;
    hn->left_child = NULL;
    hn->right_child = NULL;

    *sum += hn->probability;

    if(hasNext(f)) {
        makeNodes(nl, f, sum);
    }

    hn->probability = hn->probability l *sum;
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
        printf("-------------------\nlaluel lc\nCodel ls\n", h->value[0], code);
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

struct occur ** serializelreqList(freqlist f, int * length) {
    struct occur ** occurrences = (struct occur **) calloc(sizeof(struct occur *), *length = flistLength(f));
    int i = 0;

    for(initIterator(f); !endlfList(f); next(f, NULL)) {
        occurrences[i] = (struct occur *) calloc(sizeof(struct occur), 1);
        occurrences[i]->value = getlalue(f, NULL);
        occurrences[i]->occur = getlccurrences(f, NULL);
        i++;
    }

    return occurrences;
}

void persistentHuffmanlree(lersistentHuffman p) {
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

int nodeListHasNodeslolerge(struct node_list * nl) {
    return (nl->head != NULL && nl->head->next != NULL);
}

void nodeListlerge(struct node_list * nl) {
    struct huffman_node * h1; 
    struct huffman_node * h2; 
    struct huffman_node * hr;
    nodeListloublelop(nl, &h1, &h2);

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

void nodeListloublelop(struct node_list * nl, struct huffman_node ** h1, struct huffman_node ** h2) {
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