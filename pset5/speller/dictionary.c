// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// count the number of words in the dictionary
unsigned int word_count = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 676;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // hash the word to obtain a hash value
    int index = hash(word);

    // access linked list at that index
    // traverse the linked list, one word at a time, looking for the word using strcasecmp (case insensitive) to compare.
    // start with cursor set to first element in linked list
    // keep moving cursor until you get to null

    node *cursor = table[index];
    while (cursor != NULL)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            // found the word
            return true;
        }
        else
        {
            // move cursor to next node
            cursor = cursor->next;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int first_value = toupper(word[0]) - 'A';
    if (strlen(word) > 1)
    {
        int second_value = toupper(word[1]) - 'A';
        return first_value * 26 + second_value;
    }
    else
    {
        return first_value;
    }
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // open dictionary file
    FILE *dict = fopen(dictionary, "r");
    // check if return value is NULL, if so, function should return FALSE
    if (dict == NULL)
    {
        return false;
    }
    // printf("opened dictionary succesfuly\n");

    // set all items on node *table[N] to NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // read strings from file
    // fscanf with a loop
    char word[LENGTH];
    while (fscanf(dict, "%s", word) != EOF)
    {
        // printf("%s", word);
        // create a new node (to store a particular word inside the hash table)
        // use malloc
        node *wnode = calloc(1, sizeof(node));
        if (wnode == NULL)
        {
            return false;
        }

        // copy word into node using strcpy
        strcpy(wnode->word, word);
        // printf("%s\n", wnode->word);

        // hash word (above function). function takes a string and returns an index value
        int index = hash(wnode->word);
        // printf("%i\n", index);

        // insert word into *table[N]

        // if table[index] is empty, insert the first node into it
        // else prepend the new node
        if (table[index] == NULL)
        {
            table[index] = wnode;
            word_count++;
            // printf("inserted first\n");
        }
        else
        {
            wnode->next = table[index];
            table[index] = wnode;
            word_count++;
            // printf("inserted under\n");
        }
    }

    fclose(dict);
    // printf("dictionary loaded into memory\n");

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *cursor = NULL;
    node *temp = NULL;

    for (int i = 0; i < N; i++)
    {
        temp = table[i];
        cursor = table[i];
        while (cursor != NULL)
        {
            temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }

    return true;
}
