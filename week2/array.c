#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int n;
    do
    {
        n = get_int("Enter size of n: ");
    }
    while (n < 1);

    int array[n];
    array[0] = 1;

    for (int i = 0; i < n; i++)
    {
        array[i + 1] = array[i] * 2;
        printf("%i\n",array[i]);
    }

    // for (int i = 0; i < n; i++)
    // {
    //     printf("%i\n",array[i]);
    // }
}