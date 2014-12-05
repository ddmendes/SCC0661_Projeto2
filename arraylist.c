#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arraylist.h"

/**
 * Redimensiona o ArrayList para um cluster size a mais.
 */
void resizeUp(ArrayList a);

/**
 * Estrutura utilizada no array list.
 */
struct array_list {
	/**
	 * Array de elementos.
	 */
	elem * array;
	/**
	 * Tamanho base do array.
	 */
	int cluster_size;
	/**
	 * Quantidade de clusteres em uso.
	 */
	int cluster_units;
	/**
	 * Espaço utilizado no array.
	 */
	int array_size;
	/**
	 * Contador de operações.
	 */
	int operation_count;
	/**
	 * Valor do contador de operações quando o iterador é iniciado.
	 */
	int iterator_count;
	/**
	 * Ponteiro de navegação no vetor.
	 */
	int pointer;
};

ArrayList newArrayList(int cluster_size) {
	ArrayList a = (ArrayList) calloc(sizeof(struct array_list), 1);
	a->array_size = a->operation_count = a->iterator_count = a->pointer = 0;
	a->cluster_size = cluster_size;
	a->cluster_units = 1;
	a->array = (elem *) calloc(sizeof(elem), a->cluster_size);

	return a;
}

void arrayListAdd(ArrayList a, elem e) {
	a->operation_count++;

	if(a->array_size == a->cluster_units * a->cluster_size) {
		resizeUp(a);
	}

	a->array[a->array_size++] = e;
}

void resizeUp(ArrayList a) {
	a->array = (elem *) realloc(a->array, sizeof(elem) * a->cluster_size * (++a->cluster_units));
}

void arrayListInitIterator(ArrayList a) {
	a->iterator_count = a->operation_count;
	a->pointer = 0;
}

elem arrayListNext(ArrayList a, int * err) {
	if(a->iterator_count != a->operation_count) {
		if(err) *err = ERR_INVALID_ITERATOR;
		return 0;
	} else if(a->pointer >= a->array_size) {
		if(err) *err = ERR_END_OF_LIST;
		return 0;
	} else {
		if(err) *err = SUCCESS;
		return a->array[a->pointer++];
	}
}

int arrayListHasNext(ArrayList a) {
	return a->pointer < a->array_size;
}

int arrayListEndOfList(ArrayList a) {
	return a->pointer >= a->array_size;
}

elem * arrayListCloneArray(ArrayList a) {
	elem * arr = (elem *) calloc(sizeof(elem), a->array_size);
	arr = memcpy(arr, a->array, a->array_size * sizeof(elem));
	return arr;
}

int arrayListLength(ArrayList a) {
	return a->array_size;
}