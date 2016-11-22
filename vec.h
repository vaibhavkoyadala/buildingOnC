#ifndef VEC_H
#define VEC_H


typedef struct Vec {
    int size, cap;
    void **arr;
}Vec;
Vec* Vec_new(int cap);
void Vec_append(Vec *vec, void *data);

void** Vec_vec2Arr(Vec *vec, int *size);
void Vec_reset(Vec *vec, void(*destroyData)(void*data));
void Vec_destroy(Vec *vec, void(*destroyData)(void*data));


#endif