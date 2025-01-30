#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int main()
{
    int *array;
    int size = 10;
    array = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        array[i] = i;
        printf("%d\n", array[i]);
    }
    printf("\n");
    int new_size = 20;
    array = (int *)realloc(array, new_size * sizeof(int));
    for (int i = 0; i < new_size; i++)
    {
        array[i] = i;
        printf("%d\n", array[i]);
    }
}