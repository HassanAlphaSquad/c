#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>
#include <unistd.h>

// Node structure
typedef struct Node
{
    void *data;
    struct Node *next;
} Node;

// MPSC Queue structure
typedef struct
{
    _Atomic(Node *) head;
    Node *tail;
} MPSCQueue;

// Initialize queue
void mpsc_queue_init(MPSCQueue *q)
{
    Node *dummy = (Node *)malloc(sizeof(Node));
    dummy->next = NULL;
    q->head = dummy;
    q->tail = dummy;
}

// Enqueue (Multiple Producers)
void mpsc_enqueue(MPSCQueue *q, void *data)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;

    Node *prev_head = atomic_exchange(&q->head, new_node);
    prev_head->next = new_node;
}

// Dequeue (Single Consumer)
void *mpsc_dequeue(MPSCQueue *q)
{
    Node *tail = q->tail;
    Node *next = tail->next;

    if (next)
    {
        q->tail = next;
        free(tail);
        return next->data;
    }

    return NULL; // Queue is empty
}

// Cleanup queue
void mpsc_queue_destroy(MPSCQueue *q)
{
    while (mpsc_dequeue(q))
        ;          // Empty the queue
    free(q->tail); // Free dummy node
}

// Producer function
void *producer(void *arg)
{
    MPSCQueue *q = (MPSCQueue *)arg;
    for (int i = 0; i < 5; i++)
    {
        int *value = (int *)malloc(sizeof(int));
        *value = i;
        mpsc_enqueue(q, value);
        printf("Produced: %d\n", *value);
        usleep(100000); // Simulate work
    }
    return NULL;
}

// Consumer function
void *consumer(void *arg)
{
    MPSCQueue *q = (MPSCQueue *)arg;
    for (int i = 0; i < 10; i++)
    {
        void *data;
        while (!(data = mpsc_dequeue(q)))
        {
            usleep(50000); // Wait for data
        }
        printf("Consumed: %d\n", *(int *)data);
        free(data);
    }
    return NULL;
}

// Main function
int main()
{
    MPSCQueue queue;
    mpsc_queue_init(&queue);

    pthread_t producer1, producer2, consumer_thread;

    pthread_create(&producer1, NULL, producer, &queue);
    pthread_create(&producer2, NULL, producer, &queue);
    pthread_create(&consumer_thread, NULL, consumer, &queue);

    pthread_join(producer1, NULL);
    pthread_join(producer2, NULL);
    pthread_join(consumer_thread, NULL);

    mpsc_queue_destroy(&queue);
    return 0;
}
