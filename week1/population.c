#include <cs50.h>
#include <stdio.h>

int yearstogoal(int current, int ending)
{
    if (current == ending)
    {
        return 0;
    }

    int year = 1;

    while (current <= ending)
    {
        current = current + (current / 3) - (current / 4);
        if (current >= ending)
        {
            return year;
        }
        year++;
    }

    return -1;
}

int main(void)
{
    int start, ending;

    do
    {
        start = get_int("Start size: ");
    }
    while (start < 9);

    do
    {
        ending = get_int("End size: ");
    }
    while (ending < start);

    printf("Years: %i\n", yearstogoal(start, ending));
}
