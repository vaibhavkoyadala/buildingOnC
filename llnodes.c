#include "llnodes.h"
#include<malloc.h>
Node1* Node1_new(void *data, Node1 *next)
{
    Node1 *node = (Node1*)malloc(sizeof(Node1));
    node->data = data;
    node->next = next;
    return node;
}
Node2* Node2_new(void *data, Node2 *next, Node2 *prev)
{
    Node2 *node = (Node2*)malloc(sizeof(Node2));
    node->data = data;
    node->next = next;
    node->prev = prev;
    return node;
}