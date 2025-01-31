#include <stdio.h>
#include <malloc.h>

struct node
{
    int data;
    struct node *next;
};

void printList(struct node *singleNode){
    while(singleNode != NULL){
        printf("%d_",singleNode->data);
        singleNode = singleNode->next;
    }
}

int main()
{
    struct node *head;
    struct node *one;
    struct node *two;

    one = malloc(sizeof(struct node));
    two = malloc(sizeof(struct node));

    one->data = 10;
    two->data = 20;

    one->next = two;
    two->next = NULL;

    head = one;
    printList(head);

}