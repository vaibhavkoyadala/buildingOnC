#ifndef Set_H
#define Set_H

#include "logger.h"
#include "malloc.h"
#include "macros.h"

extern void* Set_DELETED;
extern void* Set_EMPTY;

typedef struct Set Set;

struct Set* Set_new(int size, int(*hash)(void*item), int(*equals)(void*a, void*b));
int Set_insert(Set *set, void *item);
int Set_insertIfNotPresent(Set* set, void *item);
void* Set_lookup(Set *set, void *item);
int Set_contains(Set *set, void *key);
void Set_remove(Set *set, void *item);
int Set_loadFactorPercent(Set *set);
void Set_print(Set *set, void(*printItem)(void*item));

void** SetoArray(Set *set, int *size);
int Set_size(Set *set);

typedef struct Set_iterator Set_iterator;
Set_iterator* Set_iter(Set *set);
int Set_next(Set_iterator *itr);
void* Set_item(Set_iterator *itr);

void Set_destroy(Set *set, void(*destroyItem)(void*item));

void printEmpty();

#endif