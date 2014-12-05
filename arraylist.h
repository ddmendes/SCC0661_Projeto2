
/**
 * Sem erros.
 */
#define SUCCESS 0
/**
 * ArrayList sofreu alterações desde a última inicialização de iterador.
 */
#define ERR_INVALID_ITERATOR 2
/**
 * Iterador atingiu fim da lista.
 */
#define ERR_END_OF_LIST 3

typedef struct array_list * ArrayList;
typedef char byte;
typedef byte elem;

/**
 * Criação de um novo ArrayList.
 * @param cluster_size tamanho de cada cluster do array.
 * @return novo ArrayList.
 */
ArrayList newArrayList(int cluster_size);

/**
 * Adciona um novo elemento ao ArrayList.
 * @param a ArrayList em uso.
 * @param e elemento a ser adcionado ao ArrayList.
 */
void arrayListAdd(ArrayList a, elem e);

/**
 * Inicia um iterador sobre o ArrayList.
 * @param a ArrayList sobre o qual se iniciará o iterador.
 */
void arrayListInitIterator(ArrayList a);

/**
 * Avança iterador para o próximo elemento do ArrayList.
 * @param a ArrayList em uso.
 * @param err Saida de erro.
 * @return elemento apontado antes da operação de avanço.
 */
elem arrayListNext(ArrayList a, int * err);

/**
 * Verfica se há um próximo elemento a ser recuperado.
 * @param a ArrayList em uso.
 * @return 0 caso não haja um próximo elemento, 1 caso contrário.
 */
int arrayListHasNext(ArrayList a);

/**
 * Verfica se o fim da lsita foi alcançado.
 * @param a ArrayList em uso.
 * @return 1 caso o fim da lista tenha sido alcançado, 0 caso contrário.
 */
int arrayListEndOfList(ArrayList a);

/**
 * Clona o vetor de elementos interno do ArrayList.
 * @param a ArrayList em uso.
 * @return ponteiro para novo vetor de elementos.
 */
elem * arrayListCloneArray(ArrayList a);
/**
 * Recupera o tamanho do ArrayList.
 * @param a ArrayList em uso.
 * @return Tamanho em uso do vetor de elementos.
 */
int arrayListLength(ArrayList a);