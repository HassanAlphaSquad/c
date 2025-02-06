#include <stdio.h>
#include <stdlib.h>

typedef struct node_t
{
    int value;
    struct node_t *next;
} Node;

typedef struct queue_t
{
    Node *front;
    Node *rear;
} Queue;

Queue *create_queue()
{
    Queue *queue = malloc(sizeof(Queue));
    if (!queue)
    {
        printf(" \nerror while creating queue occurred!");
        return NULL;
    }
    queue->front = queue->rear = NULL;
    return queue;
}

int null_checks(Queue *queue)
{
    if (queue == NULL)
    {
        printf(" \nQueue not intialized!");
        return -1;
    }
    if (queue->front == NULL)
    {
        printf(" \nQueue is null");
        return -1;
    }
    return 0;
}

Node *create_node(int value)
{
    Node *node = malloc(sizeof(Node));
    if (!node)
    {
        printf(" \nMemory allocation failed!\n");
        return NULL;
    }
    node->value = value;
    node->next = NULL;
    return node;
}

// Queue added element it its last (just like read life queues)
void enqueue(Queue *queue, int value)
{
    if (queue == NULL)
    {
        printf(" \nQueue not found!");
        return;
    }
    Node *new_node = create_node(value);
    if (!new_node)
    {
        printf(" \nerror ocurred while creating node!");
        return;
    }
    if (queue->rear == NULL)
    {
        queue->front = queue->rear = new_node;
        return;
    }
    queue->rear->next = new_node; // first, placing it after rear node
    queue->rear = new_node;       // then promoting new node to rear
}

// Queue remove element from first (just like read life queues)
void dequeue(Queue *queue)
{
    if (queue == NULL)
    {
        printf(" \nQueue not intialized!");
        return;
    }
    if (queue->front == NULL)
    {
        printf(" \nQueue is null");
        return;
    }
    Node *temp = queue->front;
    if (queue->front->next != NULL)
    {
        Node *new_front = queue->front->next;
        free(temp);
        queue->front = new_front;
        return;
    }
    else
    {
        queue->front = NULL;
    }
}

int count = 0;
int size(Queue *queue)
{
    if (queue == NULL)
    {
        printf("\n \n Queue not intialized!");
        return -1;
    }
    if (queue->front == NULL)
    {
        printf("\n \n Queue is null");
        return -1;
    }
    Node *current = queue->front;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }
    // printf("\n Size of queue: %d\n", count);
    return count;
}

int is_empty(Queue *queue)
{
    // return size(queue) == 0;
    if (queue->front == NULL)
        return 1;
    return queue->front == NULL;
}

void delete(Queue *queue)
{
    if (queue == NULL)
    {
        printf(" \nInvalid queue!");
    }
    if (queue->front == NULL)
    {
        printf(" \nQueue not intialized!");
    }
    Node *current = queue->front;
    Node *temp;
    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp);
    }
    queue->front = NULL;
    queue->rear = NULL;

    printf(" \nQueue deleted successfully.\n");
    return;
}

void print_queue(Queue *queue)
{
    if (queue == NULL || queue->front == NULL)
    {
        printf("Queue not intialized properly!");
        return;
    }
    printf("\n");
    Node *current = queue->front;
    while (current != NULL)
    {
        printf(" [%d] ", current->value);
        if (current->next != NULL)
        {
            printf("<= ");
        }
        current = current->next;
    }
    printf("\n");
    return;
}

Node *front(Queue *queue)
{
    null_checks(queue);
    return queue->front;
}

Node *rear(Queue *queue)
{
    null_checks(queue);
    return queue->rear;
}

int main()
{
    Queue *q = create_queue();
    enqueue(q, 10);
    enqueue(q, 20);
    enqueue(q, 30);
    // printf("%d", q->rear->value);
    // printf("%d -> ", q->front->value);
    // printf("%d -> ", q->front->next->value);
    // printf("%d -> ", q->front->next->next->value);
    // print_queue(q);
    // dequeue(q);
    dequeue(q);
    enqueue(q, 321);
    enqueue(q, 432);
    enqueue(q, 890);
    // dequeue(q);
    print_queue(q);
    char *queue_empty = is_empty(q) ? "EMPTY" : "NO";
    printf("\n Is empty? %s", queue_empty);
    size(q);
    printf("\n");
    Node *firstNode = front(q);
    Node *lastNode = rear(q);
    printf("\n First Element of queue: %d", firstNode->value);
    printf("\n Last Element of queue: %d", lastNode->value);
    // delete (q);
    // print_queue(q);
    printf("\n\n");
}