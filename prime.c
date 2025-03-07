#include <stdio.h>

int print_primes(int max)
{
    for (int i = 2; i <= max; i++)
    {
        for (int j = 2; j <= max; j++)
        {
            // printf("%d=>%d\n",i,j);
            if (i%j!=0){
                if (i==j){
                    break;
                }
                printf("%d=>%d\n",i,j);
            }
            // if (j%i!=0){
            //     if (i==j){
            //         continue;
            //     }
            //     printf("%d\n",i);
            //     // printf("%d=>%d\n",i,j);
            // }
            // printf("%d=>%d\n",i,j);

            // if ((i % j != 0))
            // {
            //     // if (i==j){
            //     //     break;
            //     // }
            //     // printf("%d=>%d\n", i,j);
            //     printf("%d\n",i);
            // }
            // // continue;
        }
    }
    printf("\n");
}

int main()
{
    print_primes(5);
    // print_primes(20);
    // print_primes(50);
}