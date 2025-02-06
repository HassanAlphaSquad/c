#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct _node
{
    int value;
    struct _node *next;
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

void print_list(LinkedList *list)
{
    if (list == NULL || list->head == NULL)
    {
        printf("List is empty. Nothing to print!\n");
        return;
    }

    Node *head = list->head;
    printf("[ ");
    while (head != NULL)
    {
        printf("%d ", head->value);
        head = head->next;
    }
    printf("]\n");
}

void push_head(LinkedList *list, int value)
{
    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
    {
        return;
    }
    new_node->value = value;
    new_node->next = list->head; // new_node's next will be HEAD, because it has the address to current node1
    list->head = new_node;
    list->length++;
    return;
}

void push_tail(LinkedList *list, int value)
{
    Node *new_node = malloc(sizeof(Node));
    new_node->value = value;

    if ((list->head) == NULL)
    {
        (list->head) = new_node;
        return;
    }
    else
    {
        Node *lastNode = (list->head); // i will take head as lastNode and put a loop to check unless the nextNode is null
        while (lastNode->next != NULL)
        {
            lastNode = lastNode->next;
        }
        lastNode->next = new_node; // when the lastNode->next will become null, it will be linked to new node (or assigned address of)
    }
}

void delete_value(LinkedList *list, int key)
{
    Node *delete_node;
    if ((list->head)->value == key)
    {
        delete_node = list->head; // we will store head in another variable and finally del that variable from mem
        list->head = (list->head)->next;
        free(delete_node);
        list->length--;
        return;
    }
    else
    {
        Node *current_node = list->head;
        while (current_node->next != NULL)
        {
            if (current_node->next->value == key)
            {
                delete_node = current_node->next;
                current_node->next = current_node->next->next;
                free(delete_node);
                list->length--;
                return;
            }
            else
            {
                current_node = current_node->next; // if condition not matched, then keep moving current pointer forward (increment)
            }
            printf("Nothing to delete. Node not found\n");
            return;
        }
    }
}

void delete_index(LinkedList *list, int index)
{
    if ((list->head) == NULL || index < 0)
    {
        printf("delete_index() failure.\n");
        return;
    }

    Node *p = (list->head);
    Node *q = ((list->head))->next;
    for (int i = 0; i < index - 1; i++)
    {
        if (q->next == NULL)
        {
            printf("delete_index() failure: The index is not accessible.\n");
            return;
        }
        else
        {
            p = p->next;
            q = q->next;
        }
    }
    p->next = q->next;
    free(q);
    list->length--;
}

void pop_head(LinkedList *list)
{
    if (list->head != NULL)
    {
        Node *delete_node;
        delete_node = list->head; // we will store head in another variable and finally del that variable from mem
        list->head = (list->head)->next;
        free(delete_node);
        list->length--;
        return;
    }
    else
    {
        printf("Invalid operation!");
        return;
    }
}

void pop_tail(LinkedList *list)
{
    Node *temp = list->head;
    Node *prev;
    if (temp == NULL)
    {
        printf("Error: List not initialized properly!");
        return;
    }
    if (temp->next == NULL)
    {
        printf("%d deleted!\n", temp->value);
        free(temp);
        list->length--;
        return;
    }
    while (temp->next != NULL)
    {
        prev = temp;
        temp = temp->next;
    }
    prev->next = NULL;
    printf("%d deleted\n", temp->value);
    free(temp);
    list->length--;
}

Node *search_list(LinkedList *list, int key)
{
    if (list == NULL)
        return NULL;

    Node *current_node = list->head;
    while (current_node != NULL)
    {
        if (current_node->value == key)
        {
            printf("Node: %d found in list\n", key);
            return current_node;
        }

        current_node = current_node->next;
    }
    printf("Node: %d not found in list\n", key);
    return NULL;
}

void search_replace(LinkedList *list, int old, int new)
{
    if (list == NULL)
        return;
    Node *n = search_list(list, old);
    if (n == NULL)
        return;
    n->value = new;
}

Node *get(LinkedList *list, int i)
{
    if (list == NULL)
        return NULL;
    if (i >= list->length)
        return NULL;

    int ci = 0;
    Node *current_node = list->head;
    while (current_node != NULL)
    {
        if (ci == i)
        {
            printf("Node: at %d found in list\n", i);
            return current_node;
        }
        current_node = current_node->next;
        ci++;
    }
    printf("Node: at %d not found in list\n", i);
    return NULL;
}

int replace(LinkedList *list, int i, int new_value)
{
    if (list == NULL)
        return NULL;
    Node *n = get(list, i);
    if (n == NULL)
        return NULL;
    int old = n->value;
    n->value = new_value;
    return old;
}

Node *new_node(int value)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL)
        return NULL;
    new_node->value = value;
    new_node->next = NULL;
    return new_node;
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

    //! In case of termination
    // if (index < 0 || index > list->length)
    // {
    //     printf("Invalid index!\n");
    //     return NULL;
    // }

    Node *node = new_node(value);
    if (!node)
        return NULL; // Memory allocation failed

    if (index == 0)
    {
        node->next = list->head;
        list->head = node;
        if (list->tail == NULL) // If inserting first node
            list->tail = node;
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
        current->next = node;

        if (node->next == NULL) // If inserted at the last position
            list->tail = node;
    }

    list->length++; // Update length dynamically
    return node;
}

void swap(int *val1, int *val2)
{
    int temp = *val1;
    *val1 = *val2;
    *val2 = temp;
}

int list_length(LinkedList *list)
{
    if (list == NULL)
        return 0;
    return list->length;
}

typedef bool (*SortPredicate)(int a, int b);

Node *bubble_sort(LinkedList *list, SortPredicate predicate)
{
    if (list->head == NULL)
        return list->head; // No sorting needed for empty or single-node list

    int swapped;
    Node *ptr;
    Node *last_sorted = NULL; // Marks the last sorted element

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
        last_sorted = ptr; // Reduce the range of checking in each pass
    } while (swapped);

    return list->head;
}

bool sort_assending(int a, int b)
{
    return a > b;
}

bool sort_dessending(int a, int b)
{
    return a < b;
}

int main()
{

    LinkedList *list = create_list();
    push_tail(list, 32);
    push_tail(list, 3542);
    push_head(list, 10);
    push_tail(list, 32);
    push_tail(list, 3);
    push_tail(list, 6);
    push_tail(list, 1);
    push_tail(list, -1);
    bubble_sort(list, sort_dessending);
    print_list(list);
    printf("\n");
    get(list, 32);
    printf("\n");
}