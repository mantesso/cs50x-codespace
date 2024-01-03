#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    char *infile = argv[1];
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 1;
    }

    FILE *output = NULL;

    typedef uint8_t BYTE;
    BYTE buffer[BLOCK_SIZE];

    int count = 0;
    while (fread(buffer, 1, BLOCK_SIZE, inptr) == BLOCK_SIZE)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (output != NULL)
            {
                fclose(output);
            }

            char filename[12];
            sprintf(filename, "%03i.jpg", count);
            output = fopen(filename, "w");
            if (output == NULL)
            {
                fprintf(stderr, "Could not create %s.\n", filename);
                fclose(inptr);
                return 1;
            }

            count++;
        }

        if (output != NULL)
        {
            fwrite(buffer, 1, BLOCK_SIZE, output);
        }
    }

    fclose(inptr);
    fclose(output);
}
