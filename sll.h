#ifndef SLL_H
#define SLL_H
#include"llnodes.h"
Node1* SLL_insertAtEnd(Node1 *head, void *data);
Node1* SLL_insertNext(Node1 *head, void *data);
void SLL_destroy(Node1 *head, void(*destroyData)(void*data));
void SLL_print(Node1 *head, void(*printData)(void*data));


#endif