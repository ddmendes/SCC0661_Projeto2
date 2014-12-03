
#define SUCCESS 0
#define ERR_INVALID_ITERATOR 2
#define ERR_END_OF_LIST 3

typedef struct array_list * ArrayList;
typedef char byte;
typedef byte elem;

ArrayList newArrayList(unsigned int cluster_size);
void arrayListAdd(ArrayList a, elem e);
void arrayListInitIterator(ArrayList a);
elem arrayListNext(ArrayList a, unsigned int * err);
int arrayListHasNext(ArrayList a);
int arrayListEndOfList(ArrayList a);
elem * arrayListCloneArray(ArrayList a);
unsigned int arrayListLength(ArrayList a);