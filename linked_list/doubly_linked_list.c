#include <stdio.h>
#include <stdlib.h>
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
    LinkedList *list = malloc(sizeof(LinkedList));
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

void head_pop(LinkedList *list)
{
    if (list == NULL || list->head == NULL)
    {
        return;
    }

    Node *temp = list->head;
    list->head = list->head->next;

    if (list->head != NULL)
    {
        list->head->prev = NULL;
    }
    else
    {
        list->tail = NULL; // If the list is empty after deletion
    }

    free(temp);
    list->length--;
}

void tail_pop(LinkedList *list)
{
    if (list == NULL || list->tail == NULL)
    {
        return;
    }

    Node *temp = list->tail;
    list->tail = list->tail->prev;

    if (list->tail != NULL)
    {
        list->tail->next = NULL;
    }
    else
    {
        list->head = NULL; // If the list is empty after deletion
    }

    free(temp);
    list->length--;
}

void delete_value(LinkedList *list, int value)
{
    if (list == NULL || list->head == NULL)
    {
        return;
    }

    Node *current = list->head;
    while (current != NULL && current->value != value) // Change 'data' to 'value'
    {
        current = current->next;
    }

    if (current == NULL) // Value not found
    {
        return;
    }

    if (current->prev != NULL)
    {
        current->prev->next = current->next;
    }
    else
    {
        list->head = current->next;
    }

    if (current->next != NULL)
    {
        current->next->prev = current->prev;
    }
    else
    {
        list->tail = current->prev;
    }

    free(current);
    list->length--;
}

void delete_index(LinkedList *list, int index)
{
    if (list == NULL || list->head == NULL || index < 0 || index >= list->length)
    {
        return;
    }

    Node *current = list->head;
    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }

    if (current->prev != NULL)
    {
        current->prev->next = current->next;
    }
    else
    {
        list->head = current->next;
    }

    if (current->next != NULL)
    {
        current->next->prev = current->prev;
    }
    else
    {
        list->tail = current->prev;
    }

    free(current);
    list->length--;
}

void delete(LinkedList *list)
{
    if (list == NULL || list->head == NULL)
    {
        return;
    }

    Node *current = list->head;
    Node *next = NULL;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

Node *get(LinkedList *list, int i)
{
    if (list == NULL)
        return NULL;

    if (i >= list->length || i < 0)
        return NULL;  // Ensure i is within the valid range

    int ci = 0;
    Node *current_node = list->head;

    // Traverse from the head to the i-th node
    while (current_node != NULL)
    {
        if (ci == i)
        {
            // printf("Node at index %d found in the list: %d\n", i, current_node->value);
            return current_node;
        }
        current_node = current_node->next;
        ci++;
    }

    // If no node is found (shouldn't really happen with the bounds check above)
    printf("Node at index %d not found in the list\n", i);
    return NULL;
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

Node* search(LinkedList *list, int value) {
    if (list == NULL) {
        return NULL;
    }
    Node* current = list->head;
    while (current != NULL) {
        if (current->value == value) {
            // printf("Node %d found", current->value);
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void search_replace(LinkedList *list, int old, int new) {
    if (list == NULL) {
        return;
    }
    Node* n = search(list, old);
    if (n == NULL) {
        return;
    }
    n->value = new;

}

int length(LinkedList *list) {
    if (list == NULL) return -1;
    return list->length;
}

void swap(int *val1, int *val2)
{
    int temp = *val1;
    *val1 = *val2;
    *val2 = temp;
}

int replace(LinkedList* list, int i, int new_value) {
    if (list == NULL)
        return -1;
    Node* n = get(list, i);
    if (n == NULL)
        return -1;
    int old = n->value;
    n->value = new_value;
    return old;
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
    insert_index(list, 17, 70);
    // print_list(list);
    // get(list, 2);
    print_list(list);
    replace(list, 1, 101);
    print_list(list);
    // head_pop(list);
    // head_pop(list);
    // tail_pop(list);
    // print_list(list);
    // printf("Length: %d\n", list->length);
    // search(list, 20);
    // search_replace(list, 20,21);
    // print_list(list);
    // // get(list, 10);
    // get_index(list, 0);
    // get(list, 20);
    // printf("Length: %d\n", length(list));
    // delete(list);
    // print_list(list);
    // printf("Length: %d\n", list->length);
    // get(list, 6);
    // get(list, 7);
    return 0;
}