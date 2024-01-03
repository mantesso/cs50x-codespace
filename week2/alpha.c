#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main(void)
{
    string input = get_string("Enter string: ");
    int length = strlen(input);

    for (int i = 0; i < length - 1; i++)
    {
        if (input[i + 1] < input[i])
        {
            printf("no\n");
            return 0;
        }
    }

    printf("yes\n");
    return 0;
}