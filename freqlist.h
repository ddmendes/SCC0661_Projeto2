
#define ERR_ITEM_NOT_FOUND 1
#define ERR_INVALID_ITERATOR 2
#define ERR_END_OF_LIST 3

typedef struct head_node * freqlist;
typedef char byte;

/**
 * Instancia uma nova lista de frequencia.
 * @return Ponteiro para nova lista de frequencias.
 */
freqlist newFreqList();

/**
 * Adciona um novo item à lista de frequencias.
 * @param f Lista de frequencias em uso.
 * @param item Item a ser adcionado a lista.
 */
void add(freqlist f, byte item);

/**
 * Inicia iterador da lista.
 * @param Lista de frequencias em uso.
 * @return retorna sempre 1.
 */
int initIterator(freqlist f);

/**
 * Recupera valor de nó durante iteração.
 * @param f Listade frequencias em uso.
 * @param err retorno de erro.
 * @return Valor do nó da iteração atual.
 */
byte getValue(freqlist f, int * err);

/**
 * Recupera número de ocorrências do nó da iteração atual.
 * @param f Lista de frequencias em uso.
 * @param err Retorno de erros.
 * @return Número de ocorrências do nó da iteração atual.
 */
int getOccurrences(freqlist f, int * err);

/**
 * Avança nó atual do iterador.
 * @param f Lista de frequencias em uso.
 * @param err Retorno de erros.
 */
void next(freqlist f, int * err);

/** 
 * Verifica se o fim da lista foi alcançado.
 * @param f Lista de frequcia em uso.
 * @return 1 caso o iterador tenha alcaçado o fim da lista, 0 caso contrário.
 */
int endOfList(freqlist f);

/**
 * Verfica se há um próximo nó a partir daiteração atual.
 * @param f Lista de frequencias em uso.
 * @return 1 caso haja um próximo elemento, 0 caso contrário.
 */
int hasNext(freqlist f);

/**
 * Recupera o tamanho da lista.
 * @param f Lita de frequencias em uso.
 * @return Número de elementos na lista.
 */
int flistLength(freqlist f);

/**
 * Exibe estado atual da lista.
 */
void flist_dump(freqlist f);