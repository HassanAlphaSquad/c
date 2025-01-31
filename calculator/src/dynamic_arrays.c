#include <stdio.h>
#include <stdlib.h>

#define MAX_HISTORY 10

typedef struct
{
    int *data;
    int capacity;
    int size;
} DynamicArray;

DynamicArray *createDynamicArray(int capacity)
{
    DynamicArray *arr = (DynamicArray *)malloc(sizeof(DynamicArray));
    arr->data = (int *)malloc(capacity * sizeof(int));
    arr->capacity = capacity;
    arr->size = 0;
    return arr;
}

void resizeArray(DynamicArray *arr, int newCapacity)
{
    int *newData = (int *)malloc(newCapacity * sizeof(int));
    for (int i = 0; i < arr->size; i++)
    {
        newData[i] = arr->data[i]; // after allocating newCapacity to array, it makes sure the data at indexes remains unchanged
    }
    free(arr->data);
    arr->data = newData;
    arr->capacity = newCapacity;
}

void insertElement(DynamicArray *arr, int element)
{
    if (arr->size == arr->capacity)
    {
        resizeArray(arr, arr->capacity * 2);
    }
    arr->data[arr->size] = element;
    arr->size++;
}

int removeElement(DynamicArray *arr)
{
    if (arr->size == 0)
    {
        printf("Error: Array is empty\n");
        return -1;
    }
    int element = arr->data[arr->size - 1];
    arr->size--;
    if (arr->size < arr->capacity / 4)
    {
        resizeArray(arr, arr->capacity / 2);
    }
    return element;
}

void freeDynamicArray(DynamicArray *arr)
{
    free(arr->data);
    free(arr);
}

int main()
{
    DynamicArray *data = createDynamicArray(1);
    insertElement(data, 1);

    for (int i = 0; i < data->size; i++)
    {
        printf("data array has: %d", data->data[i]);
        printf("\n");
    }

    printf("\n");

    int removeIndex = removeElement(data);

    printf("record removed: %d", removeIndex);

    printf("\n");
}