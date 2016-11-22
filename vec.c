#include "vec.h"
#include<malloc.h>
#include<stdio.h>
#define EXPAND_RATE 1.6

Vec* Vec_new(int cap)
{
    cap = cap <= 0 ? 10 : cap;
    Vec *vec = (Vec*)malloc(sizeof(Vec));
    vec->size = 0;
    vec->cap = cap;
    vec->arr = (void**)malloc(cap*sizeof(void*));
    return vec;
}


void _Vec_expand(Vec *vec)
{
    if (vec == NULL)    return;
    vec->cap = (int) (vec->cap * EXPAND_RATE);
    vec->arr = realloc(vec->arr, vec->cap*sizeof(void*));
    return;
}
void Vec_append(Vec *vec, void *data)
{
    if (vec == NULL)    return;
    if (vec->size == vec->cap)
        _Vec_expand(vec);

    vec->arr[vec->size++] = data;
}


void** Vec_vec2Arr(Vec *vec, int *size)
{
    if (vec == NULL){
        if(size != NULL)    *size = 0;
        return NULL;
    }
    void **arr = realloc(vec->arr, vec->size*sizeof(void*));
    if (size != NULL)    *size = vec->size;
    free(vec);
    return arr;
}



void _Vec_destroyData(void **arr, int size, void(*destroyData)(void*data))
{
    while (size--)
        destroyData(arr[size]);
}
void Vec_destroy(Vec *vec, void(*destroyData)(void*data))
{
    if (vec == NULL)    return;
    if (destroyData != NULL)
        _Vec_destroyData(vec->arr, vec->size, destroyData);

    free(vec->arr);
    free(vec);
}
void Vec_reset(Vec *vec, void(*destroyData)(void*data))
{
    if (vec == NULL)    return;
    if (destroyData != NULL)
        _Vec_destroyData(vec->arr, vec->size, destroyData);
    vec->size = 0;
}