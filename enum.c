#include <stdio.h>

enum days
{
    Sunday = 10,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
};

enum Status
{
    Not_Completed = 0,
    In_Progress = 1,
    Completed = 2
};

typedef enum
{
    RED,
    BLUE,
    GREEN,
    BLACK,
    WHITE
} COLOURS;

int main()
{
    enum days d = Monday;
    printf("your day is at index: %d", d);
    printf("\n");
    printf("task status code: %d", Completed);
    printf("\n");
    for (COLOURS c = 0; c < BLUE; c++)
    {
        printf("there is a valid colour at index # %d", c);
        printf("\n");
    }
    printf("\n");
    printf("your favourite color is at index: %d", WHITE);
    printf("\n");
}