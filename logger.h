#ifndef LOGGER_H
#define LOGGER_H
#include<stdlib.h>

#ifndef LOGGING_ON

#include<stdio.h>
#define verbose(S, ...) (fprintf(stderr, "[verbose]  : " S "\n", __VA_ARGS__))
#define   error(S, ...) (fprintf(stderr, "[ error ]  : " S " :: File=%s, Line=%d \n", __VA_ARGS__, __FILE__, __LINE__))
#define    warn(S, ...) (fprintf(stderr, "[  warn ]  : " S "\n", __VA_ARGS__))
#define   debug(S, ...) (fprintf(stderr, "[ debug ]  : " S "\n", __VA_ARGS__))

#else

#define verbose(...) 
#define verbose(...) 
#define verbose(...) 
#define verbose(...) 

#endif

#endif