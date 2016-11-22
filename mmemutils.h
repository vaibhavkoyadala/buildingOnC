#ifndef MMEMUTILS_H
#define MMEMUTILS_H

#include<stdio.h>
void mmemset(char *ptr, char ch, int size);
void mmemcopy(char *from, char *to, int n);
void fmemset(FILE *f, char ch, int size);
#endif