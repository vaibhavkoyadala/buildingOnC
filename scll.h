#ifndef SCLL_H
#define SCLL_H

#include "llnodes.h"

Node1* SCLL_insertNext(Node1 *last, void *data);
void* SCLL_remove(Node1 *node);
void SCLL_destroy(Node1 *node, void(*destroyData)(void*data));
Node1* SCLLoSLL(Node1 *last);
void SCLL_print(Node1* last, void(*printData)(void* data));

#endif