#include <stdio.h>
#include <stdlib.h>

typedef struct node_t
{
    int value;
    struct node_t *next;
} Node;

typedef struct stack_t
{
    Node *head;
} Stack;

Stack *create_stack()
{
    Stack *stack = malloc(sizeof(Stack));
    if (!stack)
    {
        printf("error while creating stack occurred!");
        return NULL;
    }
    stack->head = NULL;
    return stack;
}

Node *create_node(int value)
{
    Node *node = malloc(sizeof(Node));
    if (!node)
    {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    node->value = value;
    node->next = NULL;
    return node;
}

int is_empty(Stack *stack)
{
    return stack->head == NULL;
}

void push(Stack *stack, int value)
{
    if (stack == NULL)
    {
        printf("Stack is not initialized!\n");
        return;
    }

    Node *new_node = create_node(value);
    if (!new_node)
    {
        printf("Error while creating node!\n");
        return;
    }

    new_node->next = stack->head; // current head of stack will become next of new node
    stack->head = new_node;       // new node will become head of stack
}

Node *pop(Stack *stack)
{
    if (stack == NULL)
    {
        printf("stack not found\n");
        return NULL;
    }
    if (is_empty(stack))
    {
        printf("stack is empty\n");
        return NULL;
    }

    Node *pop_node = stack->head;    // target current head of stack for popping out
    stack->head = stack->head->next; // set node next to head as head of stack
    pop_node->next = NULL;           // delete pop_node (which has head node)
    return pop_node;
}

int peek(Stack *stack)
{

    if (!is_empty(stack))
        return stack->head->value;
    else
    {
        printf("\nStack is empty");
    }
}

int bottom(Stack *stack)
{
    if (!is_empty(stack))
    {
        Node *last = stack->head;
        while (last->next != NULL)
        {
            last = last->next;
        }
        return last->value;
    }
    else
    {
        printf("\nStack is empty");
        return -1;
    }
}

void print_stack(Stack *stack)
{
    if (stack == NULL)
    {
        printf("error with stack\n");
        return;
    }
    if (stack->head == NULL)
    {
        printf("Stack is empty\n");
        return;
    }
    printf("\n");
    Node *current = stack->head;
    while (current != NULL)
    {
        printf(" %d ", current->value);
        if (current->next != NULL)
        {
            printf("\n  ↓\n");
        }
        current = current->next;
    }
    printf("\n\n");
}

void size(Stack *stack)
{
    if (stack == NULL)
    {
        printf("there is something wrong with your stack");
        return;
    }
    if (stack->head == NULL)
    {
        printf("\nStack is empty (size is 0)\n");
        return;
    }
    int size = 0;
    Node *current = stack->head;
    while (current != NULL)
    {
        size++;
        current = current->next;
    }
    printf("\nSize of stack: %d\n", size);
    return;
}

void delete(Stack *stack)
{
    if (stack == NULL || stack->head == NULL)
    {
        return;
    }

    Node *current = stack->head;
    Node *temp;

    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp);
    }

    stack->head = NULL;
}

int main()
{
    Stack *stack = create_stack();
    push(stack, 10);
    push(stack, 20);
    push(stack, 30);
    push(stack, 40);
    push(stack, 50);
    // if (is_empty(stack))
    // {
    //     push(stack, 18430);
    //     push(stack, 35534);
    //     push(stack, 98534);
    //     push(stack, 76430);
    //     push(stack, 23340);
    // }
    // delete (stack);
    push(stack, 21);
    size(stack);
    // print_stack(stack);

    // delete (stack);
    // size(stack);
    // Node *popped = pop(stack);
    // printf("%d", popped->value);
    // printf("%d", bottom(stack));
    // print_stack(stack);
    // peek(stack);
}