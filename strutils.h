#ifndef STRUTILS_H
#define STRUTILS_H

#include<stdio.h>
#include<malloc.h>
/// Deprecated (Due to Bad design)
char* int2str(int n);
int str2int(char *str, int *error);
int len(char *s);
char* cat(char *a, char *b);
void makeUpper(char *str);
void makeLower(char *str);
int strcmp(char *a, char *b);
#endif