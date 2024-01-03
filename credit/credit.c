#include <cs50.h>
#include <stdio.h>

int sum_of_digits(int n)
{
    int sum = 0, m;
    while (n > 0)
    {
        m = n % 10;
        sum += m;
        n /= 10;
    }
    return sum;
}

int main(void)
{
    long card;

    do
    {
        card = get_long("Number: ");
    }
    while (card <= 0);

    int i = 0;
    int second_to_last = 0, last = 0;
    int digit, first, first_two;

    while (card)
    {
        // printf("  %i", i);
        // printf("%li\n", card % 10);

        // determine card type
        // if (i == )

        if (i % 2 == 0)
        {
            last += (card % 10);
        }
        else
        {
            second_to_last += sum_of_digits(card % 10 * 2);
        }

        // printf("card is %li\n", card);
        // printf("")

        // get first digit
        if (card <= 100 && card >= 10)
        {
            first_two = card;
        }
        else if (card <= 10)
        {
            first = card;
        }

        card /= 10;
        i++;
    }

    // printf("last: %i\n", last);
    // printf("second_to_last: %i\n", second_to_last);

    int verify_sum = last + second_to_last;
    // printf("verify_sum: %i\n", verify_sum);

    // printf("last is: %i\n", (verify_sum % 10));
    // printf("first_two is: %i\n", first_two);
    // printf("first is: %i\n", first);
    // printf("length i is %i\n", i);

    if (verify_sum % 10 == 0)
    {
        if ((first_two == 34 || first_two == 37) && i == 15)
        {
            printf("AMEX\n");
        }
        else if ((first_two == 51 || first_two == 52 || first_two == 53 || first_two == 54 || first_two == 55) && i == 16)
        {
            printf("MASTERCARD\n");
        }
        else if (first == 4 && (i == 13 || i == 16))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}