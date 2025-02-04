#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>

typedef struct node_t
{
    int value;
    struct node_t *prev;
    struct node_t *next;
} Node;

typedef struct
{
    Node *head;
    Node *tail;
    int length;
} LinkedList;

LinkedList *create_list()
{
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    if (!list)
    {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    return list;
}

Node *create_node(int value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node)
    {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    node->value = value;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void head_push(LinkedList *list, int value)
{
    if (list == NULL)
    {
        return;
    }
    Node *node = create_node(value);
    if (!node)
    {
        return;
    }
    node->next = list->head;
    if (list->head != NULL)
    {
        list->head->prev = node;
    }
    else
    {
        list->tail = node; // if the list is empty, head & tail both should point to new node
    }

    list->head = node;
    list->length++;
}

void tail_push(LinkedList *list, int value)
{
    if (list == NULL)
    {
        return;
    }

    Node *new_node = create_node(value);
    if (!new_node)
    {
        return;
    }

    if (list->head == NULL)
    {
        // If the list is empty, the new node becomes both the head and the tail
        list->head = new_node;
        list->tail = new_node;
    }
    else
    {
        list->tail->next = new_node; // if list already has tail, the it's next will be new_node
        new_node->prev = list->tail; // new_node will have existing tail as its previous node
        list->tail = new_node;       // Update the tail to the new node
    }

    list->length++; // Increment the length of the list
}

Node *insert_index(LinkedList *list, int index, int value)
{
    if (list == NULL)
    {
        printf("List not initialized!\n");
        return NULL;
    }

    int original_index = index;
    if (index < 0)
    {
        index = 0;
        printf("Index %d is out of bounds, clamping to %d (head).\n", original_index, index);
    }
    if (index > list->length)
    {
        index = list->length;
        printf("Index %d is out of bounds, clamping to %d (tail).\n", original_index, index);
    }

    Node *node = create_node(value);
    if (!node)
        return NULL; // Memory allocation failed

    if (index == 0)
    { // Insert at head
        node->next = list->head;
        if (list->head != NULL)
        {
            list->head->prev = node;
        }
        list->head = node;
        if (list->tail == NULL)
        {
            list->tail = node; // First node becomes both head and tail
        }
    }
    else
    {
        Node *current = list->head;
        for (int i = 0; i < index - 1 && current != NULL; i++)
        {
            current = current->next;
        }

        if (current == NULL)
        {
            printf("Invalid index!\n");
            free(node);
            return NULL;
        }

        node->next = current->next;
        node->prev = current;
        if (current->next != NULL)
        {
            current->next->prev = node;
        }
        current->next = node;

        if (node->next == NULL)
        {
            list->tail = node; // Update tail if inserted at the end
        }
    }

    list->length++; // Update length dynamically
    return node;
}

Node *get(LinkedList *list, int i)
{
    if (list == NULL || i < 0 || i >= list->length)
    {
        return NULL; // Invalid index
    }

    Node *current_node = list->head;
    for (int ci = 0; ci < i; ci++)
    {
        current_node = current_node->next;
    }

    // Print the result in the format: index => value
    printf("Node at index %d => %d\n", i, current_node->value);
    return current_node;
}

void print_list(LinkedList *list)
{
    if (list == NULL)
    {
        return;
    }
    Node *current = list->head;
    printf("[");
    while (current != NULL)
    {
        printf("%d", current->value);
        if (current->next != NULL)
        { // for avoiding trailing comma
            printf(", ");
        }
        current = current->next;
    }
    printf("]\n");
}

void print_reverse(LinkedList *list)
{
    if (list == NULL || list->tail == NULL)
    {
        return;
    }

    Node *current = list->tail;
    printf("[");

    while (current != NULL)
    {
        printf("%d", current->value);
        if (current->prev != NULL)
        {
            printf(", "); // Avoid trailing comma
        }
        current = current->prev;
    }

    printf("]\n");
}

int main()
{
    LinkedList *list = create_list();
    // head_push(list, 10);
    // head_push(list, 20);
    // head_push(list, 30);
    // tail_push(list, 40);
    // head_push(list, 1);
    // tail_push(list, 100);
    // head_push(list, 2);
    // tail_push(list, 101);
    // print_list(list);
    // print_reverse(list);
    // insert_index(list, 4, 1000);
    // insert_index(list, 1, 9999999);
    // insert_index(list, 15, 43465442);
    // print_list(list);
    // get(list, 3);
    insert_index(list, 0, 0);
    insert_index(list, 1, 10);
    insert_index(list, 2, 20);
    insert_index(list, 3, 30);
    insert_index(list, 4, 40);
    insert_index(list, 5, 50);
    insert_index(list, 6, 60);
    // print_list(list);
    get(list, 2);
    return 0;
}