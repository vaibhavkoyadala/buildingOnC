#include "set.h"

/* Tim Peter says Perturb = 5 was the most efficient */
#define PERTURB_SHIFTS 5
/* Load factor = filled buckets*100/total buckets
(Should never be 100%, because the implemented linear probing
assumes there is atleast one free bucket, also 66% is what
the Python guys use.)   */
#define MAX_LOAD_FACTOR_PERCENT 66
/* new size = old size * EXPAND_RATE */
#define EXPAND_RATE 1.6

void* Set_DELETED = &Set_DELETED;
void* Set_EMPTY = &Set_EMPTY;

typedef struct set_Entry{
    void *item;
    int hc;
}set_Entry;

struct Set{
    set_Entry *arr;
    int(*hash)(void*item);
    int(*equals)(void*a, void*b);

    int size,
        emptyBuckets,
        mask;
    
};
void _Set_init(Set *set, int size, int(*hash)(void*item), int(*equals)(void*a, void*b));
void _Set_clearTable(set_Entry *arr, int size);
void _Set_expandTable(Set *set);
int _Set_greaterGemino(int n);
int _Set_mask(int size);

struct Set* Set_new(int size, int(*hash)(void*item), int(*equals)(void*a, void*b))
{
    /* CREATE a new HashTable with the given initial size */
    if (hash == NULL || equals == NULL) return NULL;

    struct Set *set = (Set*)malloc(sizeof(Set));
    _Set_init(set, size, hash, equals);
    return set;
}

void _Set_init(Set *set, int size, int(*hash)(void*item), int(*equals)(void*a, void*b))
{
    /* INITIALIZE the HashTable */
    size = max(4, EXPAND_RATE*size);
    set->emptyBuckets = size;
    set->size = size;
    set->mask = _Set_mask(size);
    set->hash = hash;
    set->equals = equals;
    set->arr = (set_Entry*)malloc(size*(sizeof(set_Entry)));
    _Set_clearTable(set->arr, size);
}

int Set_insert(Set *set, void *item)
{
    /*  INSERT the the given item with the given item
    *   into the table.
    *   "item" and "item" can be NULL, make sure
    *   hash() and item() support it too.
    *   RETURNS 1, if inserted
    *           0, otherwise
    */

    if (set == NULL || item == NULL)	return 0;
    if (Set_loadFactorPercent(set) > MAX_LOAD_FACTOR_PERCENT)
        _Set_expandTable(set);

    // Localize frequently used params
    struct set_Entry *arr = set->arr;
    int(*equals)(void*a, void*b) = set->equals;

    int size = set->size,
        hc = set->hash(item),
        rno = set->mask & hc,
        index;
    unsigned int perturb = (unsigned)hc;

    while (1)
    {
        rno = 5 * rno + 1 + perturb;
        index = rno%size;

        if (arr[index].item == Set_EMPTY || arr[index].item == Set_DELETED)
        {
            arr[index].hc = hc;
            arr[index].item = item;
            set->emptyBuckets--;
            return 1;
        }
        else if (arr[index].hc == hc && equals(arr[index].item, item))
        {
            arr[index].item = item;
            return 1;
        }
        perturb >>= PERTURB_SHIFTS;
    }
}
int Set_insertIfNotPresent(Set* set, void *item)
{
    /*  INSERT the the given item with the given item
    *   into the table only if it is not in the table.
    *   "item" and "item" can be NULL, make sure
    *   hash() and item() support it too.
    *   RETURNS 1, if inserted
    0, otherwise
    */

    if (set == NULL || item == NULL)	return 0;
    if (Set_loadFactorPercent(set) > MAX_LOAD_FACTOR_PERCENT)
        _Set_expandTable(set);

    // Localize frequently used params
    struct set_Entry *arr = set->arr;
    int(*equals)(void*a, void*b) = set->equals;

    int size = set->size,
        hc = set->hash(item),
        rno = set->mask & hc,
        index;
    unsigned int perturb = (unsigned)hc;

    while (1)
    {
        rno = 5 * rno + 1 + perturb;
        index = rno%size;

        if (arr[index].item == Set_EMPTY || arr[index].item == Set_DELETED)
        {
            arr[index].hc = hc;
            arr[index].item = item;
            set->emptyBuckets--;
            return 1;
        }
        else if (arr[index].hc == hc && equals(arr[index].item, item))
        {
            // Already present, do not override
            return 0;
        }
        perturb >>= PERTURB_SHIFTS;
    }
}
void* Set_lookup(Set *set, void *item)
{
    /*  RETURNS the item for the given item.
    *   RETURNS Set_EMPTY if no entry for the given item is found.
    *   Set_EMPTY is a void* which is guareented to be unequal to
    *   any pointer (except itself, of course).
    */

    if (set == NULL)	return Set_EMPTY;

    // Localize frequently used params
    struct set_Entry *arr = set->arr;
    int(*equals)(void*a, void*b) = set->equals;

    
    int size = set->size,
        hc = set->hash(item),
        rno = set->mask & hc,
        index;
    unsigned int perturb = (unsigned)hc;
    while (1)
    {
        rno = 5 * rno + 1 + perturb;
        index = rno%size;
        if (arr[index].item == Set_EMPTY)
            return Set_EMPTY;
        if (arr[index].hc == hc && equals(arr[index].item, item) && arr[index].item != Set_DELETED)
            return arr[index].item;
        perturb >>= PERTURB_SHIFTS;
    }
    return Set_EMPTY;
}
int Set_contains(Set *set, void *key)
{
    if (set == NULL) return 0;
    return Set_lookup(set, key) != Set_EMPTY;
}
void Set_remove(Set *set, void *item)
{
    /*  REMOVE this entry from the hassetable. */

    if (set == NULL)	return;
    if (set->size == set->emptyBuckets)return;

    // Localize frequently used params
    struct set_Entry *arr = set->arr;
    int(*equals)(void*a, void*b) = set->equals;

    int size = set->size,
        hc = set->hash(item),
        rno = set->mask & hc,
        index;
    unsigned int perturb = (unsigned)hc;

    while (1)
    {
        rno = 5 * rno + 1 + perturb;
        index = rno%size;
        if (arr[index].item == Set_EMPTY)
            return;
        if (arr[index].hc == hc && equals(arr[index].item, item))
        {
            if (arr[index].item != Set_DELETED){
                arr[index].item = Set_DELETED;
                set->emptyBuckets++;
            }
            return;
        }
        perturb >>= PERTURB_SHIFTS;
    }
}


void** SetoArray(Set *set, int *size)
{
    if (set == NULL)    return NULL;
    int _size = Set_size(set);
    if (_size == 0) return NULL;
    void **arr = (void**)malloc(_size*sizeof(void*));
    Set_iterator *itr = Set_iter(set);
    int i = 0;
    do{
        arr[i++] = Set_item(itr);
    } while (Set_next(itr));
    if (size != NULL)   *size = _size;
    return arr;
}
int Set_size(Set *set)
{
    if (set == NULL)  return 0;
    return set->size - set->emptyBuckets;
}
int Set_loadFactorPercent(Set *set)
{
    /* RETURNS the load factor of the hash table */
    if (set == NULL)	return -1;

    return (set->size - set->emptyBuckets) * 100 / set->size;
}

void _Set_expandTable(Set *set)
{
    /*
    *   EXPAND the size of the HashTable array
    *   to oldSize*EXPAND_RATE.
    */
    if (set == NULL)	return;

    int oldSize = set->size;
    set_Entry *oldTable = set->arr;

    _Set_init(set, oldSize*EXPAND_RATE, set->hash, set->equals);

    while (oldSize--){
        if (oldTable[oldSize].item != Set_EMPTY && oldTable[oldSize].item != Set_DELETED)
            Set_insert(set, oldTable[oldSize].item);
    }

    free(oldTable);
}

void _Set_clearTable(set_Entry *arr, int size)
{
    if (arr == NULL || size < 0)  return;
    while (size--)
        arr[size].item = Set_EMPTY;
    return;
}

int _Set_greaterGemino(int n)
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

int _Set_mask(int size)
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
    int gemino = _Set_greaterGemino(size);
    return gemino & (gemino - 1);
}

void _Set_printAll(Set *set, void(*printItem)(void*item))
{
    if (set == NULL){
        printf("<null HashTable>\n");
        return;
    }
    if (printItem == NULL){
        printf("<Unable to print because printItem() == NULL>\n");
        return;
    }
    
    printf("\n{ ");
    set_Entry *arr = set->arr;
    int size = set->size;
    for (int i = 0; i < size; i++){
        if (arr[i].item == Set_EMPTY)
            printf("Empty, ", i);
        else if (arr[i].item == Set_DELETED)
            printf("Deleted, ", i);
        else{
            printItem(arr[i].item);
            printf(", ", i, arr[i].item);
        }
    }
    printf(" }\n");
}
void Set_print(Set *set, void(*printItem)(void*item))
{
    if (set == NULL){
        printf("<null HashTable>\n");
        return;
    }
    printf("{ ");
    Set_iterator *itr = Set_iter(set);
    if (itr == NULL){
        printf("<empty HashTable>\n");
        return;
    }
    do{
        printItem(Set_item(itr));
        printf(", ");
    } while (Set_next(itr));
    printf(" }\n");
}
typedef struct Set_iterator{
    int curr, toRead;
    set_Entry *table;
}Set_iterator;
Set_iterator* Set_iter(Set *set)
{
    if (set == NULL)
        return NULL;
    if (set->emptyBuckets == set->size)
        return NULL;
    struct Set_iterator *iterator = (Set_iterator*)malloc(sizeof(Set_iterator));
    iterator->toRead = set->size - set->emptyBuckets;
    iterator->table = set->arr;
    iterator->curr = -1;
    Set_next(iterator);
    return iterator;
}
int Set_next(Set_iterator *itr)
{
    if (itr == NULL)    return 0;
    if (itr->toRead == 0)   return 0;
    struct set_Entry *table = itr->table;
    int curr = itr->curr;
    do {
        curr++;
    } while (table[curr].item == Set_EMPTY || table[curr].item == Set_DELETED);
    itr->curr = curr;
    itr->toRead--;
    return 1;
}
void* Set_item(Set_iterator *itr)
{
    if (itr == NULL)    return NULL;

    return itr->table[itr->curr].item;
}
void Set_destroy(Set *set, void(*destroyItem)(void*item))
{
    if (set == NULL)    return;
    if (destroyItem != NULL){
        Set_iterator *itr = Set_iter(set);
        do{
            destroyItem(Set_item(itr));
        } while (Set_next(itr));
    }
    free(set->arr);
    free(set);
}
void printEmpty(){
    printf("[SET] empty=%p", Set_EMPTY);
}
/*
#pragma warning(disable : 4996)
#include<stdio.h>
#include<conio.h>
typedef struct Data{
    int a, b;
}Data;
Data* newData(int a, int b){
Data *item = (Data*)malloc(sizeof(Data));
    item->a = a;
    item->b = b;
    return item;
}
void printData(Data *item){
    if (item == NULL){
        printf("\n<null MyData>");
        return;
    }
    if (item == Set_EMPTY){
        printf("\nEmpty");
        return;
    }
    printf("(%d, %d)", item->a, item->b);
}
void hashData(Data *item)
{
    if (item == NULL)   return -1;
    return item->a * 47 + item->b;
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
    printf("deleted = %p, empty = %p", Set_DELETED, Set_EMPTY);
    Set *set = Set_new(3, hashData, equalsData);
    Data *d12 = newData(1, 2);
    Data *d23 = newData(2, 3);
    Data *d13 = newData(1, 3);
    Data *d42 = newData(4, 2);
    Set_insert(set, d12);
    Set_insert(set, d23);
    Set_remove(set, d23);
    Set_insert(set, d23);
    Set_insert(set, d42);

    Set_print(set, printData);
    getch();
}
*/