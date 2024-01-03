#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
float coleman_index(string text);
void grade(float coleman);

int main(void)
{
    string text = get_string("Text: ");
    // printf("number of letters is: %i\n", count_letters(text));
    // printf("number of words is: %i\n", count_words(text));
    // printf("number of sentences is: %i\n", count_sentences(text));
    // printf("Grade: %f\n", coleman_index(text));
    grade(coleman_index(text));
}

int count_letters(string text)
{
    int count = 0;
    int string_length = strlen(text);
    for (int i = 0; i < string_length; i++)
    {
        if (isalpha(text[i]))
        {
            count += 1;
        }
    }
    return count;
}

int count_words(string text)
{
    int count = 0;
    int string_length = strlen(text);
    for (int i = 0; i < string_length; i++)
    {
        if (isspace(text[i]))
        {
            count += 1;
        }
    }
    return count + 1;
}

int count_sentences(string text)
{
    int count = 0;
    int string_length = strlen(text);
    for (int i = 0; i < string_length; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            count += 1;
        }
    }
    return count;
}

float coleman_index(string text)
{
    float letters = (float) count_letters(text);
    float words = (float) count_words(text);
    float sentences = (float) count_sentences(text);

    float L = letters / words * 100.00;
    float S = sentences / words * 100.00;

    return 0.0588 * L - 0.296 * S - 15.8;
}

void grade(float coleman)
{
    if (coleman < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (coleman > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int) round(coleman));
    }
}