#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

// struct alignment
// double free
// void** (pointer referencing to another pointer)
// total size of struct is sum of sizes of data type typedef [struct {int *data(8 bytes because pointer is always 64 bits); int size(4 bytes); int capacity(4 bytes);} Array;] => 16 bytes
// size of struct of any program is not same over different devices.

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
    // nullCheck(array);

    if (array == NULL || array->data == NULL)
    {
        printf("Error: Array not initialized properly!\n");
        return;
    }

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
    if (array == NULL || array->data == NULL)
    {
        printf("Error: Array not initialized properly!\n");
        return;
    }

    if (array->size == array->capacity)
    {
        resizeArray(array, array->capacity + 1);
    }
    array->data[array->size] = data;
    array->size++;
}

int getIndex(Array *array, int index)
{
    if (array == NULL || array->data == NULL)
    {
        printf("Error: Array not initialized properly!\n");
        return -1;
    }
    if (index >= array->size)
    {
        printf("Index out of bound\n");
        return -1;
    }
    int dataToGet = array->data[index];
    printf("Index no. %d => Value [%d]", index, array->data[index]);
    printf("\n");
}

void getAllData(Array *array)
{
    if (array == NULL || array->data == NULL)
    {
        printf("Error: Array not initialized properly!\n");
        return;
    }
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

void getSize(Array *array)
{
    if (array == NULL || array->data == NULL)
    {
        printf("Error: Array not initialized properly!\n");
        return;
    }
    printf("Array's size: %d\n", array->size);
}

void getCapacity(Array *array)
{
    if (array == NULL || array->data == NULL)
    {
        printf("Error: Array not initialized properly!\n");
        return;
    }
    printf("Array's capacity: %d\n", array->capacity);
}

void removeElement(Array *array, int index)
{
    if (array == NULL || array->data == NULL)
    {
        printf("Error: Array not initialized properly!\n");
        return;
    }
    if (index < 0 || index > array->size)
    {
        printf("Invalid index");
    }
    for (int i = index; i < array->size - 1; i++)
    {
        array[index] = array[array->size + 1];
    }
    array->size--;
}

void safeFree(void **ptr)
{
    if (ptr && *ptr != NULL)
    {
        free(*ptr);
        *ptr = NULL;
    }
}

void freeMemory(Array *array)
{
    safeFree((void **)&array->data);
}

int main()
{
    Array *test = createArray(1);
    insertElement(test, 10);
    insertElement(test, 20);
    insertElement(test, 30);
    insertElement(test, 40);
    insertElement(test, 50);
    insertElement(test, 60);
    resizeArray(test, 50);
    printf("\n");
    // freeMemory(test);
    getAllData(test);
    getSize(test);
    // insertElement(test, 232);
    printf("\n\n");
    // getCapacity(test);
    removeElement(test, 2);
    getAllData(test);
    getSize(test);
    printf("\n\n");
    // getCapacity(test);
    insertElement(test, 43);
    getAllData(test);
    getSize(test);
    printf("\n");
    // getCapacity(test);
    // resizeArray(test, 1);

    // insertElement(test, 50);

    // printf("Size of array: %d & Capacity is: %d", test->size, test->capacity);
    // printf("\n");

    // resizeArray(test, 20);
    // printf("\nCapacity of array: %d", test->size);
    // getAllData(test);
    // getIndex(test, 1);
    // getAllData(test);
    // getIndex(test, 452);
    // getAllData(test);

    // removeElement(test, 2);

    // getAllData(test);

    // freeMemory(test);
    // printf("Size of array: %d", test->size);

    // Array *array1 = createArray(2);
    // insertElement(array1, 1);
    // insertElement(array1, 2);
    // insertElement(array1, 3);
    // insertElement(array1, 42);
    // insertElement(array1, 42);

    // getAllData(array1);
    // getSize(array1);
    // getCapacity(array1);

    // Array *students = createArray(3);
    // insertElement(students, 10);
    // insertElement(students, 44);
    // insertElement(students, 4);

    // getAllData(students);
    // getSize(students);
    // getCapacity(students);

    // insertElement(students, 23232);
    // getAllData(students);
    // getSize(students);
    // getCapacity(students);

    // Array *arr = createArray(21);
    // insertElement(arr, 2);
    // insertElement(arr, 2);
    // getSize(arr);
    // getCapacity(arr);

    printf("\n");
}