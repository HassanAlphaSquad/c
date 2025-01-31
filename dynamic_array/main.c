#include <stdio.h>
#include <malloc.h>

typedef struct
{
    int *data;
    int size;
    int capacity;
} Array;

Array *createArray(int capacity)
{
    Array *array = (Array *)malloc(sizeof(Array)); // intializing arrayay
    array->data = (int *)malloc(capacity * sizeof(int));
    array->capacity = capacity;
    array->size = 0; // new array will be empty
    return array;
}

void resizeArray(Array *array, int newCapacity)
{
    int *newData = (int *)malloc(newCapacity * sizeof(int));
    for (int i = 0; i <= array->size; i++)
    {
        newData[i] = array->data[i]; // moving previous data to new memory location
    }
    free(array->data);
    array->data = newData;
    array->capacity = newCapacity;
}

void insertElement(Array *array, int data)
{
    if (array->size == array->capacity)
    {
        resizeArray(array, array->capacity + 5);
    }
    array->data[array->size] = data;
    array->size++;
}

int getIndex(Array *array, int index)
{
    if (index >= array->size)
    {
        printf("Index out of bound\n");
        return -1;
    }
    int dataToGet = array->data[index];
    // return dataToGet;
    printf("Index no. %d => Value [%d]", index, array->data[index]);
    printf("\n");
}

void getAllData(Array *array)
{
    printf("Array contains these elements: ");
    for (int i = 0; i < array->size; i++)
    {
        printf("%d", array->data[i]);
        if (i < array->size - 1)
        {
            printf(",");
        }
        // char seperator = maxSizeReached ? ' ' : ',';
        // printf("%d%c", array->data[i], seperator);
    }
    printf("\n");
}

void freeMemory(Array *array)
{
    free(array->data);
    free(array);
}

void removeElement(Array *array, int index)
{
}

void getSize(Array *array)
{
    printf("Array's size: %d\n", array->size);
}

void getCapacity(Array *array)
{
    printf("Array's capacity: %d\n", array->capacity);
}

int main()
{
    Array *test = createArray(1);
    insertElement(test, 10);
    insertElement(test, 20);
    insertElement(test, 30);
    insertElement(test, 40);
    insertElement(test, 50);
    // insertElement(test, 50);

    printf("Size of array: %d", test->size);
    printf("\n");

    // resizeArray(test, 20);
    // printf("\nCapacity of array: %d", test->size);
    // getAllData(test);
    // getIndex(test, 1);
    // getAllData(test);
    getIndex(test, 452);
    getAllData(test);

    freeMemory(test);
    // printf("Size of array: %d", test->size);

    Array *array1 = createArray(2);
    insertElement(array1, 1);
    insertElement(array1, 2);
    insertElement(array1, 3);
    insertElement(array1, 42);

    getAllData(array1);
    getSize(array1);
    getCapacity(array1);

    printf("\n");
}