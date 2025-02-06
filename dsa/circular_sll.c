#include <stdio.h>
#include <stdlib.h>

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

void push_head(LinkedList *list, int value)
{
    Node *node = create_node(value);
    if (!node)
    {
        printf("Error: node wasn't created properly!");
        return;
    }
    node->value = value;
    if (list->head == NULL)
    {
        list->head = node;
    }
    node->next = list->head;
    if (list->head->next != NULL)
    {
        node->next = NULL;
    }
}

// void add_node_to_tail(Node **head, int value)
// {
//     Node *new_node = (Node *)malloc(sizeof(Node));
//     if (!new_node)
//     {
//         printf("Memory allocation failed!\n");
//         return;
//     }
//     new_node->value = value;
//     new_node->next = NULL;

//     if (*head == NULL)
//     {
//         // If the list is empty, the new node points to itself
//         *head = new_node;
//         new_node->next = *head; // Complete the circular link
//     }
//     else
//     {
//         // If the list is not empty, find the last node and link it to the new node
//         Node *last = *head;
//         while (last->next != *head)
//         {
//             last = last->next;
//         }
//         last->next = new_node;
//         new_node->next = *head; // Complete the circular link
//     }
// }

// Function to print the circular list
void print_circular_list(LinkedList *list)
{
    if (list->head == NULL)
    {
        printf("List is empty!\n");
        return;
    }

    Node *current = list->head;
    printf("[");

    // Traverse the list until we reach the list->head again
    do
    {
        printf("%d", current->value);
        current = current->next;

        // Print a comma if it's not the last node
        if (current != list->head)
        {
            printf(", ");
        }
    } while (current != list->head); // Stop when we circle back to the head

    printf("]\n");
}

// Function to delete a node with a given value from the circular list
void delete_node(Node **head, int value)
{
    if (*head == NULL)
    {
        printf("List is empty!\n");
        return;
    }

    Node *current = *head;
    Node *previous = NULL;

    // Traverse the list to find the node to delete
    do
    {
        if (current->value == value)
        {
            if (previous == NULL)
            {
                // If the node to delete is the head
                Node *last = *head;
                // Find the last node
                while (last->next != *head)
                {
                    last = last->next;
                }
                // Update the head
                *head = current->next;
                // Update the last node's next pointer to the new head
                last->next = *head;
            }
            else
            {
                previous->next = current->next;
            }

            free(current);
            printf("Node with value %d deleted.\n", value);
            return;
        }
        previous = current;
        current = current->next;
    } while (current != *head); // Stop if we come back to the head

    printf("Node with value %d not found.\n", value);
}

int main()
{
    LinkedList *list = create_list();
    push_head(list, 10);
    print_circular_list(list);
    // Node *head = NULL;
    // add_node_to_tail(&head, 10);
    // add_node_to_tail(&head, 20);
    // add_node_to_tail(&head, 40);

    // print_circular_list(head);

    printf("\n");
}