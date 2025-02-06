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

void push_head(LinkedList *list, int value)
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

void push_tail(LinkedList *list, int value)
{
    if (list == NULL)
    {
        printf("Error: List not initialized!");
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

    // Handle out-of-bounds index cases
    if (index <= 0)
    {
        push_head(list, value);
        return list->head;
    }

    if (index >= list->length)
    {
        push_tail(list, value);
        return list->tail;
    }

    Node *node = create_node(value);
    if (!node)
        return NULL;

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

    list->length++; // Update length dynamically
    return node;
}

void pop_head(LinkedList *list)
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

void pop_tail(LinkedList *list)
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

void clear(LinkedList *list)
{
    if (list == NULL)
        return;

    Node *current = list->head;
    while (current != NULL)
    {
        Node *next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

// void destroy(LinkedList **list)
// {
//     if (list == NULL || *list == NULL)
//     {
//         printf("List not found in memory\n");
//         return;
//     }

//     free(*list);
//     *list = NULL;

//     printf("List successfully destroyed\n");
// }

Node *get(LinkedList *list, int i)
{
    if (list == NULL)
        return NULL;

    if (i >= list->length || i < 0)
        return NULL; // if i is not within range

    int ci = 0;
    Node *current_node = list->head;

    // Traverse from the head i node
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

    // If no node is found
    printf("Node at index %d not found in the list\n", i);
    return NULL;
}

void print_list(LinkedList *list)
{
    if (list == NULL || list->head == NULL)
    {
        printf("[]\n");
        return;
    }

    Node *current = list->head;
    printf("[");

    while (current != NULL)
    {
        printf("%d", current->value);
        if (current->next != NULL)
            printf(", ");

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

Node *search(LinkedList *list, int value)
{
    if (list == NULL)
    {
        return NULL;
    }
    Node *current = list->head;
    while (current != NULL)
    {
        if (current->value == value)
        {
            // printf("Node %d found", current->value);
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void search_replace(LinkedList *list, int old, int new)
{
    if (list == NULL)
    {
        return;
    }
    Node *n = search(list, old);
    if (n == NULL)
    {
        return;
    }
    n->value = new;
}

int length(LinkedList *list)
{
    if (list == NULL)
        return -1;
    return list->length;
}

int replace(LinkedList *list, int i, int new_value)
{
    if (list == NULL)
        return -1;
    Node *n = get(list, i);
    if (n == NULL)
        return -1;
    int old = n->value;
    n->value = new_value;
    return old;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

bool sort_ascending(int a, int b)
{
    return a > b;
}

bool sort_descending(int a, int b)
{
    return a < b;
}

typedef bool (*SortPredicate)(int a, int b);

Node *bubble_sort(LinkedList *list, SortPredicate predicate)
{
    if (list->head == NULL)
        return list->head;

    int swapped;
    Node *ptr;
    Node *last_sorted = NULL;

    do
    {
        swapped = 0;
        ptr = list->head;

        while (ptr->next != last_sorted)
        {
            if (predicate(ptr->value, ptr->next->value))
            {
                swap(&(ptr->value), &(ptr->next->value));
                swapped = 1;
            }
            ptr = ptr->next;
        }
        last_sorted = ptr;
    } while (swapped);

    return list->head;
}

int main()
{
    LinkedList *list = create_list();
    // push_head(list, 10);
    // push_head(list, 20);
    // push_head(list, 30);
    // push_tail(list, 40);
    // push_head(list, 1);
    // push_tail(list, 100);
    // push_head(list, 2);
    // push_tail(list, 101);
    // print_list(list);
    // print_reverse(list);
    // insert_index(list, 4, 1000);
    // insert_index(list, 1, 9999999);
    // insert_index(list, 15, 43465442);
    // print_list(list);
    // get(list, 3);
    // insert_index(list, 0, 0);
    // insert_index(list, 1, 10);
    // insert_index(list, 2, 20);
    // insert_index(list, 3, 30);
    // insert_index(list, 4, 40);
    // insert_index(list, 5, 50);
    // insert_index(list, 6, 60);
    // insert_index(list, 17, 70);
    // print_list(list);
    // get(list, 2);
    // print_list(list);
    // replace(list, 1, 101);
    // print_list(list);
    // clear(list);
    // insert_index(list, 0, 10);
    // print_list(list);
    insert_index(list, 0, 13);
    insert_index(list, 1, 11);
    insert_index(list, 2, 1000);
    insert_index(list, 3, 12);
    print_list(list);
    // clear(list);
    // print_list(list);
    bubble_sort(list, sort_descending);
    print_list(list);
    printf("Head: %d", list->head->value);
    printf(" -> %d", list->head->next->value);
    printf(" -> %d", list->head->next->next->value);
    printf(" -> %d", list->head->next->next->next->value);
    printf("\n");
    // insert_index(list, 34, 321);
    insert_index(list, 8, 888);
    insert_index(list, 0, 32);
    insert_index(list, -1, 121);
    print_list(list);
    // printf("\nTail: %d", list->tail->value);
    // printf(" -> %d", list->tail->prev->value);
    // printf(" -> %d", list->tail->prev->prev->value);
    // printf(" -> %d", list->tail->prev->prev->prev->value);
    // destroy(list);
    // print_list(list);
    // destroy(&list);
    // print_list(list);

    // pop_head(list);
    // pop_head(list);
    // pop_tail(list);
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
}