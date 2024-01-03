#include <cs50.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, string argv[])
{
    // check number of arguments
    // (should have one argument which is the file to check)
    if (argc != 2)
    {
        printf("wrong usage\n");
        return 1;
    }


    // open the file
    string filename = argv[1];
    FILE *input = fopen(filename, "r");

    // check if file exists
    if (input == NULL)
    {
        printf("no such file\n");
        return 1;
    }

    uint8_t buffer[4];
    uint8_t signature[] = {37, 80, 68, 70};

    fread(buffer, 1, 4, input);
    fclose(input);

    for (int i = 0; i < 4; i++)
    {
        if (buffer[i] != signature[i])
        {
            printf("likely not a pdf\n");
            return 0;
        }
    }

    printf("likely a pdf!\n");
    return 0;

}

