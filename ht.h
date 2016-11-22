#ifndef HT_H
#define HT_H

#include "logger.h"
#include "malloc.h"
#include "macros.h"

extern void* HT_EMPTY;
typedef struct HT HT;

struct HT* HT_new(int size, int(*hash)(void*key), int(*equals)(void*a, void*b));
int HT_insert(HT *ht, void *key, void *data);
int HT_insertIfNotPresent(HT* ht, void *key, void*data);
void* HT_lookup(HT *ht, void *key);
int HT_contains(HT *ht, void* key);
void HT_remove(HT *ht, void *key);
int HT_loadFactorPercent(HT *ht);
void HT_print(HT *ht, void(*printKey)(void*key), void(*printData(void*data)));


typedef struct HT_iterator HT_iterator;
HT_iterator* HT_iter(HT *ht);
int HT_next(HT_iterator *itr);
void* HT_key(HT_iterator *itr);
void* HT_data(HT_iterator *itr);

#endif