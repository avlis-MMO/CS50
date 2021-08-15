// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include "dictionary.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <strings.h>


// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 33;

int count_words = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //Get hash index
    int index = hash(word);
    node *cursor = table[index];
    
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //djb2 hash function
    unsigned int hash = 5381;
    int c;
    while ((c = tolower(*word++)))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //Open dict
    FILE *file = fopen(dictionary, "r");

    if (file == NULL)
    {
        printf("Coulnd´t open file");
        return false;
    }

    char word[LENGTH + 1];
    int i = 0;

    while (fscanf(file, "%s", word) != EOF)
    {
        count_words ++;

        //Create new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        
        //Copy word to node
        strcpy(n->word, word);
        n->next = NULL;
        
        //Get hash index
        int index = hash(word);

        if (table[index] == NULL)
        {
            table[index] = n;
        }

        else
        {
            n->next = table[index];
            table[index] = n;

        }

    }
    
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (count_words != 0)
    {
        return count_words;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        node *temp = table[i];
        
        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(temp);
            temp = cursor;
        }
    }
    return true;
}
