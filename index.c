#include <stdio.h>

void Int(int value)
{
    printf("You entered an integer: %d\n", value);
}

void Float(float value)
{
    printf("You entered a float: %.4f\n", value);
}

void String(char str[])
{
    printf("You entered a string: %s\n", str);
}

int main()
{
    int intValue;
    float floatValue;
    char str[100];

    printf("Enter any value: ");

    if (scanf("%f", &floatValue))
    {
        Float(floatValue);
    }

    else if (scanf("%s", str))
    {
        String(str);
    }

    else
    {
        scanf("%d", &intValue);
        Int(intValue);
    }
}
