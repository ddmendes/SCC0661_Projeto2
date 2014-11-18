#include <stdlib.c>

#ifndef _FREQLIST_H_
    #include <freqlist.h>
    #define _FREQLIST_H_
#endif

struct node {
    byte value;
    unsigned int occurrences;
    struct node * next;
};

struct head_node {
    struct node * head;
    struct node * pointer;
    struct node * back_pointer;
    struct node * tail;
};

freqlist newFreqList() {
    freqlist f = (freqlist) calloc(sizeof(struct head_node), 1);
    f->head = NULL;
    f->pointer = NULL;
    f->back_pointer = NULL;
    f->tail = NULL;
    return f;
}

void add(freqlist f, byte item) {
    byte err = 0;
    find(f, item, &err);

    if(err == 0) {
        
        f->pointer->occurrences++;
        if(f->pointer->occurrences > f->back_pointer->occurrences)
            reorder(f);

    } else {
        
        f->pointer = (struct node *) calloc(sizeof(struct node), 1);
        f->pointer->value = item;
        f->pointer->occurrences = 1;
        f->pointer->next = NULL;
        put(f, f->pointer);

    }
    
}

void reorder(freqlist f) {
    f->back_pointer->next = f->pointer->next;
    put(f, f->pointer);
}

void put(freqlist f, struct node * toPut) {
    f->back_pointer = NULL;
    f->pointer = NULL;

    while(f->pointer != NULL) {
        if(toPut->value >= f->pointer->value)
            break;
    }

    toPut->next = f->pointer;
    if(f->back_pointer) {
        f->back_pointer->next = toPut;
    } else {
        f->head = toPut;
    }
}

byte find(freqlist f, byte item, byte * err) {
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
