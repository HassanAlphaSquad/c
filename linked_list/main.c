// Create datatype for node (1. data, 2. next node)
// Create head
// Create current

//! For insertion of node at start
// Initialize node
// Assign data
// Assign next node

#include <stdio.h>
#include <malloc.h>

struct node
{
    int data;
    struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

void addNode(int data)
{
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    newNode->data = data;
    newNode->next = head;
    head = newNode;
}

void printNodes()
{
    struct node *p = head; // starting from beginning
    while (p != NULL)
    {
        printf("%d_", p->data);
        p = p->next; // moving forward
    }
}

int main()
{
    addNode(10);
    addNode(20);
    addNode(30);
    addNode(40);
    printNodes();
}