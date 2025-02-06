#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>

// Atomic pointer to an integer
_Atomic(int *) atomic_ptr;

// Function to update the atomic pointer
void *update_pointer(void *arg)
{
    int *new_value = (int *)malloc(sizeof(int)); // Allocate memory
    *new_value = 42;                             // Set a new value

    // Atomically set the atomic pointer to the new value
    atomic_store(&atomic_ptr, new_value);

    printf("Pointer updated to: %d\n", *new_value);
    return NULL;
}

// Function to read the atomic pointer
void *read_pointer(void *arg)
{
    // Atomically load the value of the atomic pointer
    int *value = atomic_load(&atomic_ptr);

    if (value != NULL)
    {
        printf("Pointer read: %d\n", *value);
    }
    else
    {
        printf("Pointer is NULL.\n");
    }

    return NULL;
}

int main()
{
    pthread_t thread1, thread2;

    // Initially, atomic pointer is NULL
    atomic_ptr = NULL;

    // Create two threads: one to update the pointer, and one to read the pointer
    pthread_create(&thread1, NULL, update_pointer, NULL);
    pthread_create(&thread2, NULL, read_pointer, NULL);

    // Wait for threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Clean up: free the allocated memory
    int *value = atomic_load(&atomic_ptr);
    if (value != NULL)
    {
        free(value); // Free the allocated memory for the atomic pointer
    }

    return 0;
}
