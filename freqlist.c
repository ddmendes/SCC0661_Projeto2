#include <stdio.h>
#include <stdlib.h>
#include "freqlist.h"

/**
 * No de uma lista de frequencias.
 */
struct node {
    /**
     * Byte contado no nó.
     */
    byte value;
    /**
     * Ocorrências do byte contado no nó.
     */
    int occurrences;
    /**
     * Encadeamento da lista.
     */
    struct node * next;
};

/**
 * Nó sentinela da estrutura.
 */
struct head_node {
    /**
     * Ponteiro para cabeça da lista.
     */
    struct node * head;
    /**
     * Ponteiro de navegação na lista;
     */
    struct node * pointer;
    /**
     * Ponteiro atrasado de navegação na lista.
     */
    struct node * back_pointer;
    /**
     * Ponteiro para calda da lista.
     */
    struct node * tail;
    /**
     * Contador de operações.
     */
    int operation_sum;
    /**
     * Estado do contador de operações no início do iterador.
     */
    int iterator_sum;
};

/**
 * Reordena elemento apontado por pointer.
 * @param f Lista de frequencias em uso.
 */
void reorder(freqlist f);

/**
 * Enfileira um nó no fim da lista.
 */
void append(freqlist f, struct node * toAppend);

/**
 * Insere ordenadamente um nó na lista.
 * @param f Lista de frequencia em uso.
 * @param toPut Nó a ser inserido na lista.
 */
void put(freqlist f, struct node * toPut);

/**
 * Busca um elemento na lista.
 * @param f Lista de frequencia em uso.
 * @param item Item a ser buscado na lista.
 * @param err retorno de erro.
 * @return 1 em caso de sucesso na busca, 0 caso contrário.
 */
byte find(freqlist f, byte item, int * err);

freqlist newFreqList() {
    freqlist f = (freqlist) calloc(sizeof(struct head_node), 1);
    f->head = NULL;
    f->pointer = NULL;
    f->back_pointer = NULL;
    f->tail = NULL;
    f->operation_sum = 0;
    return f;
}

void add(freqlist f, byte item) {
    f->operation_sum++;
    int err = 0;
    find(f, item, &err);
    if(err == 0) {
        f->pointer->occurrences++;
        if(f->back_pointer != NULL && f->pointer->occurrences > f->back_pointer->occurrences)
            reorder(f);
    } else {
        f->pointer = (struct node *) calloc(sizeof(struct node), 1);
        f->pointer->value = item;
        f->pointer->occurrences = 1;
        f->pointer->next = NULL;
        append(f, f->pointer);

    }
}

byte find(freqlist f, byte item, int * err) {
    f->pointer = f->head;
    f->back_pointer = NULL;
    
    while(f->pointer != NULL) {
       if(f->pointer->value == item)
           break;
       f->back_pointer = f->pointer;
       f->pointer = f->pointer->next;
    }

    if(f->pointer == NULL) {
        if(err != NULL)
            *err = ERR_ITEM_NOT_FOUND;
        return (byte) 0;
    } else {
        return (byte) 1;
    }
}

void reorder(freqlist f) {
    f->back_pointer->next = f->pointer->next;
    put(f, f->pointer);
}

void append(freqlist f, struct node * toAppend) {
    if(f->tail == NULL) {
        f->head = f->tail = toAppend;
    } else {
        f->tail = f->tail->next = toAppend;
    }
}

void put(freqlist f, struct node * toPut) {
    f->pointer = f->head;
    f->back_pointer = NULL;

    while(f->pointer != NULL) {
        if(toPut->occurrences >= f->pointer->occurrences)
            break;
        f->back_pointer = f->pointer;
        f->pointer = f->pointer->next;
    }

    if(f->back_pointer == NULL) {
        f->head = toPut;
    } else {
        f->back_pointer->next = toPut;
    }

    toPut->next = f->pointer;
}

int initIterator(freqlist f) {
    f->iterator_sum = f->operation_sum;
    f->pointer = f->head;
    return 1;
}

byte getValue(freqlist f, int * err) {

    if( f->iterator_sum != f->operation_sum ) {
        if(err) *err = ERR_INVALID_ITERATOR;
        return 0;
    }

    return f->pointer->value;

}

int getOccurrences(freqlist f, int * err) {

    if( f->iterator_sum != f->operation_sum ) {
        if(err) *err = ERR_INVALID_ITERATOR;
        return 0;
    }    

    if(err) *err = 0;
    return f->pointer->occurrences;

}

void next(freqlist f, int * err) {

    if( f->iterator_sum != f->operation_sum ) {
        if(err) *err = ERR_INVALID_ITERATOR;
    } else if(f->pointer->next != NULL) {
        if(err) *err = 0;
    } else {
        if(err) *err = ERR_END_OF_LIST;
    }

    f->pointer = f->pointer->next;

}

int endOfList(freqlist f) {
    return f->pointer == NULL;
}

int hasNext(freqlist f) {
    return f->pointer != f->tail;//NULL &&  f->pointer->next != NULL;
}


void flist_dump(freqlist f) {
    f->pointer = f->head;
    while(f->pointer != NULL) {
        printf("-------------------------\n");
        printf("byte: %x\n", f->pointer->value);
        printf("occurrences: %d\n", f->pointer->occurrences);
        f->pointer = f->pointer->next;
    }
    printf("-------------------------\n");
}

int flistLength(freqlist f) {
    int l;
    f->pointer = f->head;
    for(l = 0; f->pointer != NULL; l++) {
        f->pointer = f->pointer->next;
    }
    return l;
}