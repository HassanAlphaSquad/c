// Example: Two independent threads
#include <stdio.h>
#include <pthread.h>

void *task1(void *arg)
{
    // for (int i = 0; i < 5; i++)
    // {
    printf("Task 1");
    // }
    return NULL;
}

void *task2(void *arg)
{
    // for (int i = 0; i < 5; i++)
    // {
    printf("Task 2\n");
    // }
    return NULL;
}

void *task3()
{
    printf("\nTask 3\n");
    return NULL;
}

// void *inputs()
// {
//     for (int i = 0; i < 3; i++)
//     {
//         scanf("Enter value%d: \n", &i);
//     }
// }

int main()
{
    pthread_t thread1, thread2, thread3;
    pthread_t inputs;

    pthread_create(&inputs, NULL, inputs, NULL);
    pthread_join(inputs, NULL);

    // Create two threads
    // pthread_create(&thread1, NULL, task1, NULL);
    // pthread_create(&thread2, NULL, task2, NULL);
    // pthread_create(&thread3, NULL, task3, NULL);

    // Join threads (wait for them to finish)
    // pthread_join(thread1, NULL);
    // pthread_join(thread3, NULL);
    // pthread_join(thread2, NULL);

    // printf("Both tasks completed.\n");
    printf("\n\n");

    return 0;
}
