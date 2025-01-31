#include <stdio.h>
#include <malloc.h>

struct node
{
    int data;
    struct node *next;
};

void printNodes(struct node *singleNode)
{
    printf("[");
    while (singleNode != NULL)
    {
        printf("%d ", singleNode->data);
        singleNode = singleNode->next;
    }
    printf("]");
    printf("\n");
}

struct node *head = NULL;

void addNodeAtStart(int data)
{
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    newNode->data = data;
    newNode->next = head;
    head = newNode;
}

void addNodeAtEnd(struct node **head, int data)
{ // ** because it is pointing to head which is also a pointer (to node structure)
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    newNode->data = data;
    newNode->next = NULL; // as it will be the last node, so there is no next node

    if (*head == NULL)
    {
        *head = newNode; // if there is no node before, then it will become first nod
        return;
    }
    struct node *temp_head = *head;
    while (temp_head->next != NULL)
    {
        temp_head = temp_head->next;
    }
    temp_head->next = newNode;
}

void printAllNode()
{
    // printing nodes always starts from head
    struct node *nodes = head;
    printf("[");
    while (nodes != NULL)
    {
        printf("%d ", nodes->data);
        nodes = nodes->next;
    }
    printf("]");
}

int main()
{
    struct node *one;
    struct node *two;
    struct node *three;
    struct node *four;
    struct node *five;

    one = malloc(sizeof(struct node));
    two = malloc(sizeof(struct node));
    three = malloc(sizeof(struct node));
    four = malloc(sizeof(struct node));
    five = malloc(sizeof(struct node));

    one->data = 10;
    two->data = 20;
    three->data = 30;
    four->data = 40;
    five->data = 50;

    one->next = two;
    two->next = three;
    three->next = four;
    four->next = five;
    five->next = NULL;

    head = one;
    printNodes(head);
    addNodeAtStart(100);
    addNodeAtStart(120);
    addNodeAtStart(140);
    addNodeAtStart(160);
    addNodeAtStart(180);
    addNodeAtStart(200);
    addNodeAtEnd(&head, 1000);
    printAllNode();
}