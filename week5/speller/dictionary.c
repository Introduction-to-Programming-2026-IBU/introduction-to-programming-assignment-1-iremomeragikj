#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // for strcasecmp

#include "dictionary.h"

#define N 1000

typedef struct node
{
    char word[LENGTH + 1]; // the dictionary word (+ null terminator)
    struct node *next;     // pointer to next node in bucket
} node;

node *table[N] = {NULL};
unsigned int word_count = 0;

unsigned int hash(const char *word)
{
    unsigned long hash_val = 5381;
    for (int i = 0; word[i] != '\0'; i++)
    {
        hash_val = ((hash_val << 5) + hash_val) + tolower(word[i]);
    }
    return hash_val % N;
}

bool load(const char *dictionary)
{
    // TODO 2a: open the dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];
    while (fscanf(file, "%45s", word) == 1)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fclose(file);
            return false;
        }
        strcpy(n->word, word);

        unsigned int index = hash(word);

        n->next = table[index];
        table[index] = n;

        word_count++;
    }
    fclose(file);
    return true;
}

bool check(const char *word)
{
    unsigned int index = hash(word);
    for (node *tmp = table[index]; tmp != NULL; tmp = tmp->next)
    {
        if (strcasecmp(tmp->word, word) == 0)
        {
            return true;
        }
    }
    return false;
}

unsigned int size(void)
{
    return word_count;
}

bool unload(void)
{
    // TODO: loop over all N buckets
    for (int i = 0; i < N; i++)
    {
        node *tmp = table[i];
        while (tmp != NULL)
        {
            node *next = tmp->next;
            free(tmp);
            tmp = next;
        }
        table[i] = NULL;
    }
    return true;
}
