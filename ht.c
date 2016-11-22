#include "ht.h"

/* Tim Peter says Perturb = 5 was the most efficient */
#define PERTURB_SHIFTS 5
/* Load factor = filled buckets*100/total buckets 
(Should never be 100%, because the implemented linear probing 
assumes there is atleast one free bucket, also 66% is what 
the Python guys use.)   */
#define MAX_LOAD_FACTOR_PERCENT 66
/* new size = old size * EXPAND_RATE */
#define EXPAND_RATE 1.6

void* HT_DELETED = &HT_DELETED;
void* HT_EMPTY = &HT_EMPTY;

typedef struct ht_Entry_t{
    int hc;
	void *key, *data;
}ht_Entry_t;

struct HT{
	int size,
		emptyBuckets,
        mask;
    int(*hash)(void*key);
    int(*equals)(void*a, void*b);
	ht_Entry_t *arr;
};
void HT_init(HT *ht, int size, int(*hash)(void*key), int(*equals)(void*a, void*b));
void HT_clearTable(ht_Entry_t *arr, int size);
void HT_expandTable(HT *ht);
int HT_greaterGemino(int n);
int HT_mask(int size);

struct HT* HT_new(int size, int(*hash)(void*key), int(*equals)(void*a, void*b))
{
    /* CREATE a new HashTable with the given initial size */
    if (hash == NULL || equals == NULL) return NULL;

    struct HT *ht = (HT*)malloc(sizeof(HT));
    HT_init(ht, size, hash, equals);
    return ht;
}

void HT_init(HT *ht, int size, int(*hash)(void*key), int(*equals)(void*a, void*b))
{
    /* INITIALIZE the HashTable */
    size = max(4, EXPAND_RATE*size);
    ht->emptyBuckets = size;
    ht->size = size;
    ht->mask = HT_mask(size);
    ht->hash = hash;
    ht->equals = equals;
    ht->arr = (ht_Entry_t*)malloc(size*(sizeof(ht_Entry_t)));
    HT_clearTable(ht->arr, size);
}

int HT_insert(HT *ht, void *key, void *data)
{
    /*  INSERT the the given data with the given key
    *   into the table.
    *   "key" and "data" can be NULL, make sure 
    *   hash() and data() support it too.
    *   RETURNS 1, if inserted
    *           0, otherwise
    */

	if (ht == NULL || key == NULL)	return 0;
    if (HT_loadFactorPercent(ht) > MAX_LOAD_FACTOR_PERCENT)
        HT_expandTable(ht);

    // Localize frequently used params
    struct ht_Entry_t *arr = ht->arr;
    int(*equals)(void*a, void*b) = ht->equals;
    
    unsigned int perturb = (unsigned)key;
    int size = ht->size,
        hc = ht->hash(key),
        rno = ht->mask & hc,
        index;
    
    while (1)
    {
        rno = 5 * rno + 1 + perturb;
        index = rno%size;
        
        if (arr[index].data == HT_EMPTY || arr[index].data == HT_DELETED)
        {
            arr[index].hc = hc;
            arr[index].data = data;
            arr[index].key = key;
            ht->emptyBuckets--;
            return 1;
        }else if (arr[index].hc == hc && equals(arr[index].key, key))
        {
            arr[index].data = data;
            return 1;
        }
        perturb >>= PERTURB_SHIFTS;
    }
}
int HT_insertIfNotPresent(HT* ht, void *key, void*data)
{
    /*  INSERT the the given data with the given key
    *   into the table only if it is not in the table.
    *   "key" and "data" can be NULL, make sure
    *   hash() and data() support it too.
    *   RETURNS 1, if inserted
                0, otherwise
    */

    if (ht == NULL || key == NULL)	return 0;
    if (HT_loadFactorPercent(ht) > MAX_LOAD_FACTOR_PERCENT)
        HT_expandTable(ht);

    // Localize frequently used params
    struct ht_Entry_t *arr = ht->arr;
    int(*equals)(void*a, void*b) = ht->equals;

    unsigned int perturb = (unsigned)key;
    int size = ht->size,
        hc = ht->hash(key),
        rno = ht->mask & hc,
        index;

    while (1)
    {
        rno = 5 * rno + 1 + perturb;
        index = rno%size;

        if (arr[index].data == HT_EMPTY || arr[index].data == HT_DELETED)
        {
            arr[index].hc = hc;
            arr[index].data = data;
            arr[index].key = key;
            ht->emptyBuckets--;
            return 1;
        }
        else if (arr[index].hc == hc && equals(arr[index].key, key))
        {
            // Already present, do not override
            return 0;
        }
        perturb >>= PERTURB_SHIFTS;
    }
}
void* HT_lookup(HT *ht, void *key)
{
    /*  RETURNS the data for the given key. 
    *   RETURNS HT_EMPTY if no entry for the given key is found.
    *   HT_EMPTY is a macro which is guareented to be unequal to 
    *   any pointer, except itself.
    */

    if (ht == NULL)	return HT_EMPTY;
    
    // Localize frequently used params
    struct ht_Entry_t *arr = ht->arr;
    int(*equals)(void*a, void*b) = ht->equals;

    unsigned int perturb = (unsigned)key;
    int size = ht->size,
        hc = ht->hash(key),
        rno = ht->mask & hc,
        index;
    while (1)
    {
        rno = 5 * rno + 1 + perturb;
        index = rno%size;
        if (arr[index].data == HT_EMPTY)
            return HT_EMPTY;
        if (arr[index].hc == hc && equals(arr[index].key, key) && arr[index].data != HT_DELETED)
            return arr[index].data;
        perturb >>= PERTURB_SHIFTS;
    }
}
int HT_contains(HT *ht, void *key)
{
    if (ht == NULL) return 0;
    return HT_lookup(ht, key) != HT_EMPTY;
}
void HT_remove(HT *ht, void *key)
{
    /*  REMOVE this entry from the hashtable. */

	if (ht == NULL)	return;
	if (ht->size == ht->emptyBuckets)return;

    // Localize frequently used params
    struct ht_Entry_t *arr = ht->arr;
    int(*equals)(void*a, void*b) = ht->equals;

    unsigned int perturb = (unsigned)key;
    int size = ht->size,
        hc = ht->hash(key),
        rno = ht->mask & hc,
        index;
    while (1)
    {
        rno = 5 * rno + 1 + perturb;
        index = rno%size;
        if (arr[index].data == HT_EMPTY)
            return;
        if (arr[index].hc == hc && equals(arr[index].key, key))
        {
            if (arr[index].data != HT_DELETED){
                arr[index].data = HT_DELETED;
                ht->emptyBuckets++;
            }
            return;
        }
        perturb >>= PERTURB_SHIFTS;
    }
}

int HT_loadFactorPercent(HT *ht)
{
    /* RETURNS the load factor of the hash table */
	if (ht == NULL)	return -1;

	return (ht->size - ht->emptyBuckets) * 100 / ht->size;
}

void HT_expandTable(HT *ht)
{
    /*
    *   EXPAND the size of the HashTable array
    *   to oldSize*EXPAND_RATE.
    */
	if (ht == NULL)	return;

	int oldSize = ht->size;
	ht_Entry_t *oldTable = ht->arr;
    
    HT_init(ht, oldSize*EXPAND_RATE, ht->hash, ht->equals);

	while (oldSize--){
        if (oldTable[oldSize].data != HT_EMPTY && oldTable[oldSize].data != HT_DELETED)
			HT_insert(ht, oldTable[oldSize].key, oldTable[oldSize].data);
	}

	free(oldTable);
}

void HT_clearTable(ht_Entry_t *arr, int size)
{
    if (arr == NULL || size < 0)  return;
    while (size--)
        arr[size].data = HT_EMPTY;
    return;
}

int HT_greaterGemino(int n)
{
    /*
    *   A Gemino is a number of the form 2^i.
    *   (Trivia: Name inspired from the Gemino Curse)
    *   RETURNS the first gemino after n.
    *   e.g 1->1
    *       2->2
    *       3->4
    *       89->128
    *       -10->1
    */
    if (n < 0)    return 1;
    int gemino = 1;
    while (gemino < n)
        gemino = gemino << 1;
    return gemino;
}

int HT_mask(int size)
{
    /*
    *   RETURNS the mask to be used 
    *   to find the position of an item
    *   in the array.
    *   The mask's max value should 
    *   be greater or equal to "size".
    *   Otherwise, only the lower part 
    *   of the array be getting hits
    *   (and eventually misses).
    */
    
    // Use the lower bits for the mask.
    int gemino = HT_greaterGemino(size);
    return gemino & (gemino - 1);
}

void HT_printAll(HT *ht, void (*printKey)(void*key), void(*printData(void*data)))
{
    if (ht == NULL){
        printf("<null HashTable>\n");
        return;
    }
    if (printKey == NULL){
        printf("Unable to print because printKey() == NULL\n");
        return;
    }
    if (printData == NULL){
        printf("Unable to print because printData() == NULL\n");
        return;
    }
    printf("\nindex \t key \t data");
    ht_Entry_t *arr = ht->arr;
    int size = ht->size;
    for (int i = 0; i < size; i++){
        if (arr[i].data == HT_EMPTY)
            printf("\n%d \t -- \t Empty", i);
        else if (arr[i].data == HT_DELETED)
            printf("\n%d \t -- \t Deleted", i);
        else{
            printf("\n%d \t ", i, arr[i].key);
            printKey(arr[i].key);
            printData(arr[i].data);
        }
    }
}
void HT_print(HT *ht, void(*printKey)(void*key), void(*printData(void*data)))
{
    if (ht == NULL){
        printf("<null HashTable>\n");
        return;
    }
    HT_iterator *itr = HT_iter(ht);
    if (itr == NULL){
        printf("<empty HashTable>\n");
        return;
    }
    do{
        printKey(HT_key(itr));
        printf(": ");
        printData(HT_data(itr));
        printf("\n");
    } while (HT_next(itr));
}
/*	A Java/python inspired iterator for the HashTable
	Look at HT_print() for usage.
*/

typedef struct HT_iterator{
    int curr, toRead;
    ht_Entry_t *table;
}HT_iterator;
HT_iterator* HT_iter(HT *ht)
{
    if (ht == NULL)
        return NULL;
    if (ht->emptyBuckets == ht->size)
        return NULL;
    struct HT_iterator *iterator = (HT_iterator*)malloc(sizeof(HT_iterator));
    iterator->toRead = ht->size - ht->emptyBuckets;
    iterator->table = ht->arr;
    iterator->curr = 0;
    HT_next(iterator);
    return iterator;
}
int HT_next(HT_iterator *itr)
{
    if (itr == NULL)    return 0;
    if (itr->toRead == 0)   return 0;
    struct ht_Entry_t *table = itr->table;
    int curr = itr->curr;
    do {
        curr++;
    } while (table[curr].data == HT_EMPTY || table[curr].data == HT_DELETED);
    itr->curr = curr;
    itr->toRead--;
    return 1;
}
void* HT_key(HT_iterator *itr)
{
    if (itr == NULL)    return NULL;

    return itr->table[itr->curr].key;
}
void* HT_data(HT_iterator *itr)
{
    if (itr == NULL)    return NULL;

    return itr->table[itr->curr].data;
}
//===============================================================
//======================= TESTING ===============================
//===============================================================
/*
#pragma warning(disable : 4996)
#include<stdio.h>
#include<conio.h>
typedef struct Data{
	int a, b;
}Data;
Data* newData(int a, int b){
	Data *data = (Data*)malloc(sizeof(Data));
	data->a = a;
	data->b = b;
	return data;
}
void printData(Data *data){
	if (data == NULL){
		printf("\n<null MyData>");
		return;
	}
	if (data == HT_EMPTY){
		printf("\nEmpty");
		return;
	}
    printf("(%d, %d)", data->a, data->b);
}
void hashData(Data *data)
{
    if (data == NULL)   return -1;
    return data->a * 47 + data->b;
}
int equalsData(Data *a, Data *b)
{
    if (a == b) return 1;
    if (a == NULL || b == NULL)
        return 0;
    return a->a == b->a && a->b == b->b;
}
void main()
{
    HT *ht = HT_new(3, hashData, equalsData);
    Data *d12 = newData(1, 2);
    Data *d23 = newData(2, 3);
    Data *d13 = newData(1, 3);
    Data *d42 = newData(4, 2);
    HT_insert(ht, d12, d12);
    HT_insert(ht, d23, d23);
    HT_insert(ht, d13, d13);
    HT_insert(ht, d42, d42);

    HT_print(ht, printData, printData);
    getch();
}
*/