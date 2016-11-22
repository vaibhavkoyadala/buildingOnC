#include "sll.h"
#include<stdio.h>
#include<malloc.h>

Node1* SLL_insertAtEnd(Node1 *head, void *data)
{
    if (head == NULL)
        return Node1_new(data, NULL);
    struct Node1 *iter = head;
    while (iter->next != NULL) iter = iter -> next;
    iter->next = Node1_new(data, NULL);
    return head;
}
Node1* SLL_insertNext(Node1 *head, void *data)
{
    if (head == NULL)
        return Node1_new(data, NULL);
    head->next = Node1_new(data, head->next);
    return head;
}
void _SLL_destroyWithData(Node1 *head, void(*destroyData)(void*data))
{
    Node1 *next = head->next;
    while (next != NULL){
        destroyData(head->data);
        free(head);

        head = next;
        next = next->next;
    }
    destroyData(head->data);
    free(head);
}
void _SLL_destroyOnlyNodes(Node1 *head)
{
    Node1 *next = head->next;
    while (next != NULL){
        free(head);

        head = next;
        next = next->next;
    }
    free(head);
}
void SLL_destroy(Node1 *head, void(*destroyData)(void*data))
{
    if (head == NULL)   return;
    if (destroyData == NULL)
        _SLL_destroyWithData(head, destroyData);
    else
        _SLL_destroyOnlyNodes(head);
}
void SLL_print(Node1 *head, void(*printData)(void*data))
{
    if (head == NULL){
        printf("<null SLL>");
        return;
    }
    while (head != NULL){
        printData(head->data);
        head = head->next;
    }
}