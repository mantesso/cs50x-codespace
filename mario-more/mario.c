#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;

    do
    {
        height = get_int("Height: ");
    }
    while (height <= 0 || height > 8);

    for (int i = 1; i <= height; i++)
    {
        for (int h = height - i; h >= 1; h--)
        {
            printf(" ");
        }

        for (int j = 1; j <= i; j++)
        {
            printf("#");
        }

        printf("  ");

        for (int k = 1; k <= i; k++)
        {
            printf("#");
        }
        printf("\n");
    }
}