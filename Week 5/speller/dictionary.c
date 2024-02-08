// Implements a dictionary's functionality

#include "dictionary.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    node *curr;
    int bucket = tolower(word[0]) - 97;

    curr = table[bucket];
    while (curr != NULL)
    {
        if (strcasecmp(curr->word, word) == 0)
            return true;
        else
            curr = curr->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    char str[LENGTH];
    int bucket;
    node *chain;

    FILE *rfile = fopen(dictionary, "r");

    if (rfile == NULL)
    {
        return false;
    }
    while (fscanf(rfile, "%s", str) != EOF)
    {
        chain = malloc(sizeof(node));
        strcpy(chain->word, str);
        chain->next = NULL;

        bucket = ((int) tolower(str[0])) - 97;
        if (table[bucket] == NULL)
            table[bucket] = chain;
        else
        {
            node *curr;
            curr = table[bucket];
            while ((curr->next) != NULL)
            {
                curr = curr->next;
            }
            curr->next = chain;
        }
    }
    fclose(rfile);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    unsigned int number = 0;
    node *curr;

    for (int i = 0; i < N; i++)
    {
        curr = table[i];
        while (curr != NULL)
        {
            number += 1;
            curr = curr->next;
        }
    }
    return number;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *curr;
    node *next;
    for (int i = 0; i < N; i++)
    {
        curr = table[i];
        while (curr != NULL)
        {
            next = curr->next;
            free(curr);
            curr = next;
        }
    }

    return true;
}
