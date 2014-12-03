
#define ERR_ITEM_NOT_FOUND 1
#define ERR_INVALID_ITERATOR 2
#define ERR_END_OF_LIST 3

typedef struct head_node * freqlist;
typedef char byte;

freqlist newFreqList();
void add(freqlist f, byte item);
int initIterator(freqlist f);
byte getValue(freqlist f, int * err);
unsigned int getOccurrences(freqlist f, int * err);
void next(freqlist f, int * err);
int endOfList(freqlist f);
int hasNext(freqlist f);
unsigned int flistLength(freqlist f);
void flist_dump(freqlist f);