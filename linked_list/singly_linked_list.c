#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct _node
{
    int data;
    struct _node *next;
} Node;

void print_nodes(Node *head)
{
    if (head != NULL)
    {
        printf("[");
        while (head != NULL)
        {
            printf("%d ", head->data);
            head = head->next;
        }
        printf("]");
    }
    else
    {
        printf("List is empty. Nothing to print!");
    }
    printf("\n");
}

void push_head(Node **head, int data) // double * because we want to utilize the *head (pointer) from main
{
    Node *new_node = malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = *head; // new_node's next will be HEAD, because it has the address to current node1
    *head = new_node;
    return;
}

void push_tail(Node **head, int data)
{
    Node *new_node = malloc(sizeof(Node));
    new_node->data = data;

    if (*head == NULL)
    {
        *head = new_node;
        return;
    }
    else
    {
        Node *lastNode = *head; // i will take head as lastNode and put a loop to check unless the nextNode is null
        while (lastNode->next != NULL)
        {
            lastNode = lastNode->next;
        }
        lastNode->next = new_node; // when the lastNode->next will become null, it will be linked to new node (or assigned address of)
    }
}

void delete_by_value(Node **head, int key)
{
    Node *node_to_delete;
    if ((*head)->data == key)
    {
        node_to_delete = *head; // we will store head in another variable and finally del that variable from mem
        *head = (*head)->next;
        free(node_to_delete);
        return;
    }
    else
    {
        Node *current_node = *head;
        while (current_node->next != NULL)
        {
            if (current_node->next->data == key)
            {
                node_to_delete = current_node->next;
                current_node->next = current_node->next->next;
                free(node_to_delete);
                return;
            }
            else
            {
                current_node = current_node->next; // if condition not matched, then keep moving current pointer forward (increment)
            }
            printf("Nothing to delete. Node not found\n");
        }
    }
}

void delete_by_index(Node **head, int index)
{
    if (*head == NULL || index < 0)
    {
        printf("delete_by_index() failure.\n");
        return;
    }

    Node *p = *head;
    Node *q = (*head)->next;
    for (int i = 0; i < index - 1; i++)
    {
        if (q->next == NULL)
        {
            printf("delete_by_index() failure: The index is not accessible.\n");
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
}

void pop_head(Node **head)
{
    if (head != NULL)
    {
        Node *node_to_delete;
        node_to_delete = *head; // we will store head in another variable and finally del that variable from mem
        *head = (*head)->next;
        free(node_to_delete);
        return;
    }
    else
    {
        printf("Invalid operation!");
        return;
    }
}

void pop_tail(Node **head)
{
    Node *temp = *head;
    Node *prev;
    if (temp == NULL)
    {
        printf("Error: List not initialized properly!");
        return;
    }
    if (temp->next == NULL)
    {
        printf("%d deleted!\n", temp->data);
        free(temp);
        return;
    }
    while (temp->next != NULL)
    {
        prev = temp;
        temp = temp->next;
    }
    prev->next = NULL;
    printf("%d deleted\n", temp->data);
    free(temp);
}

void search_node(Node **head, int key)
{
    bool nodeFound = false;
    if ((*head)->data == key)
    {
        nodeFound = true;
        printf("Node: %d found in list\n", key);
        return;
    }
    else
    {
        Node *current_node = *head;
        while (current_node->next != NULL)
        {
            if (current_node->next->data == key)
            {
                nodeFound = true;
                printf("Node: %d found in list\n", key);
                return;
            }
            else
            {
                current_node->next = current_node->next->next;
            }
        }
        printf("Node: %d not found in list\n", key);
    }
}

Node *create_new_node(int data)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

int get_list_length(Node *head)
{
    int count = 0;
    Node *current = head;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }
    return count;
}

Node *insert_node_at_index(Node **head, int position, int data)
{
    if (((*head) == NULL) && position > 0)
    {
        printf("List not initialized!\n");
        return NULL;
    }
    if (position < 0)
    {
        printf("Index must be greater than or equal to 0\n");
        return NULL;
    }

    Node *new_node = create_new_node(data);
    if (!new_node)
        return NULL; // Memory allocation failed

    if (position == 0)
    {
        new_node->next = *head;
        *head = new_node;
        // printf("Node: %d added at position: %d\n", data, position);
        return new_node;
    }

    Node *current = *head;
    for (int i = 0; i < position - 1 && current != NULL; i++)
    {
        current = current->next;
    }

    if (current == NULL)
    {
        printf("Invalid index!\n");
        free(new_node); // Prevent memory leak
        return NULL;
    }

    new_node->next = current->next;
    current->next = new_node;
    // printf("Node: %d added at position: %d\n", data, position);
    return new_node;
}

void swap(int *val1, int *val2)
{
    int temp = *val1;
    *val1 = *val2;
    *val2 = temp;
}

Node *bubbleSort(Node **head)
{
    if (*head == NULL)
        return *head; // No sorting needed for empty or single-node list

    int swapped;
    Node *ptr;
    Node *last_sorted = NULL; // Marks the last sorted element

    do
    {
        swapped = 0;
        ptr = *head;

        while (ptr->next != last_sorted)
        {
            if (ptr->data > ptr->next->data)
            {
                swap(&(ptr->data), &(ptr->next->data));
                swapped = 1;
            }
            ptr = ptr->next;
        }
        last_sorted = ptr; // Reduce the range of checking in each pass
    } while (swapped);

    return *head;
}

int main()
{
    Node *head = NULL;
    insert_node_at_index(&head, 0, 50);
    insert_node_at_index(&head, 1, 10);
    insert_node_at_index(&head, 2, 40);
    insert_node_at_index(&head, 3, 90);
    insert_node_at_index(&head, 4, 85);
    printf("Linked List (Original): ");
    print_nodes(head);

    bubbleSort(&head);
    printf("Linked List (Sorted): ");
    print_nodes(head);

       printf("\n");
}