#include<stdio.h>
#include<malloc.h>
#include "scll.h"

Node1* SCLL_insertNext(Node1 *last, void *data)
{  
    Node1 *newNode = Node1_new(data, NULL);
    
    if (last == NULL){
        newNode->next = newNode;
        return newNode;
    }
    else{
        newNode->next = last->next;
        last->next = newNode;
        return newNode;
    }
}
void* SCLL_remove(Node1 *node)
{
    /**
        REMOVE this node from the Circular Linked List.
        RETURNS: The data in the node.
        "node": Any node in the Circular Linked List to be removed.
        
    */
    if (node == NULL)   return NULL;
    // 1) Find the parent
    struct Node1 *parent = node;
    while (parent->next != node)
        parent = parent->next;
    // 2) Alter links
    parent->next = node->next;
    // 3) Backup the data
    void *data = node->data;
    // 4) Free the node
    free(node);
    // 5) Return the data
    return data;
}


void _SCLL_destroyWithData(Node1 *node, void (*destroyData)(void*data))
{
    if (node == NULL)   return;
    struct Node1 *next = node->next;
    node->next = NULL;
    node = next;
    next = next->next;
    while (next != NULL){
        destroyData(node->data);
        free(node);
        node = next;
        next = next->next;
    }
    destroyData(node->data);
    free(node);
}
void _SCLL_destroyOnlyNodes(Node1 *node)
{
    if (node == NULL)   return;
    struct Node1 *next = node->next;
    node->next = NULL;
    node = next;
    next = next->next;
    while (next != NULL){
        free(node);
        node = next;
        next = next->next;
    }
    free(node);
}
void SCLL_destroy(Node1 *node, void(*destroyData)(void*data))
{
    if (node == NULL)   return;
    if (destroyData == NULL)
        _SCLL_destroyWithData(node, destroyData);
    else
        _SCLL_destroyOnlyNodes(node);
}
Node1* SCLLoSLL(Node1 *last)
{
    if (last == NULL)   return NULL;
    struct Node1 *head = last->next;
    last->next = NULL;
    return head;
}
void SCLL_print(Node1* last, void(*printData)(void* data))
{
    if (last == NULL || printData == NULL){
        printf("<null Circular Linked List>");
        return;
    }
    if (last->next == NULL){
        printData(last->data);
        return;
    }
    Node1 *iter = last->next;
    while (iter != last){
        printData(iter->data);
        iter = iter->next;
    }
    printData(last->data);
}

// ============================================
// =============== Testing ====================
//=============================================
/*
#pragma warning(disable : 4996)
#include<stdlib.h>
typedef struct MyData{
    int val;
}MyData;
MyData* newMyData(int val)
{
    MyData *myData = (MyData*)malloc(sizeof(MyData));
    myData->val = val;
    return myData;
}
void printData(void *data)
{
    printf(" %d ", ((MyData*)data)->val);
}
void main()
{
    int number = 12345;
    Node1 *last = NULL;
    while (number){
        last = insert_SCLL(last, newMyData(number % 10));
        number /= 10;
    }
    print_SCLL(last, printData);
    printf("Removed: %d", ((MyData*)remove_SCLL(last->next))->val);
    print_SCLL(last, printData);
    getch();
}
*/