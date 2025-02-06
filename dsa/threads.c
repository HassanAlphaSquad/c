#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define ITERATION 2000

void *task1()
{
    for (int i = 1; i <= ITERATION * 3; i++)
    {
        printf("Task 1 => [%d]\n", i);
    }
    // printf("\n");
    // usleep(1);
}

void *task2()
{
    for (int i = 1; i <= ITERATION * 3; i++)
    {
        printf("Task 2 => [%d]\n", i);
    }
    // printf("\n");
}

void *task3()
{
    for (int i = 1; i <= ITERATION * 3; i++)
    {
        printf("Task 3 => [%d]\n", i);
    }
    // printf("\n");
}

void *task4()
{
    for (int i = 1; i <= ITERATION * 3; i++)
    {
        printf("Task 4 => [%d]\n", i);
    }
    // printf("\n");
}

void *task5()
{
    for (int i = 1; i <= ITERATION * 3; i++)
    {
        printf("Task 5 => [%d]\n", i);
    }
    // printf("\n");
}

// void *print_hello(char *txt){
//     printf("%c from a thread!\n",txt );
// }

int main()
{
    pthread_t thread1, thread2, thread3, thread4, thread5;

    pthread_create(&thread1, NULL, task1, NULL);
    pthread_create(&thread2, NULL, task2, NULL);
    pthread_create(&thread3, NULL, task3, NULL);
    pthread_create(&thread4, NULL, task4, NULL);
    pthread_create(&thread5, NULL, task5, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    pthread_join(thread5, NULL);

    // pthread_join
}