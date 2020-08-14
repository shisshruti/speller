// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

//#define N 10000

const long N = 80387;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
//const unsigned int N = 65536;

// Hash table
node *table[N];

// Number of words in the dictionary
int num = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //creates a copy of the word
    int n = strlen(word);
    char word_copy[LENGTH + 1];

    for (int i = 0; i < n; i++)
    {
        word_copy[i] = tolower(word[i]);
    }

    //adds null terminator to end string
    word_copy[n] = '\0';

    //initialises index for hashed word
    int index  = hash(word_copy);

    //sets cursor to point to same address as hashtable index
    node *cursor = table[index];

    //while cursor does not point to NULL, search the dictionary for word
    while (cursor != NULL)
    {

        //if strcasecmp returns 0, the word has been found
        if (strcasecmp(cursor->word, word_copy) == 0)
        {
            return true;
        }

        //else, word has not yet been found, so the cursor is advanced
        else
        {
            cursor = cursor->next;
        }
    }

    //cursor has reached the end of list and the word has not been found
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    
    //hash function posted on reddit by delipity

    unsigned int hash = 0;

    for (int i = 0, n = strlen(word); i < n; i++)
    {
        hash = (hash << 2) ^ word[i];
    }
    return hash % N; 
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{

    //opening the dictionary file
    FILE *dict = fopen(dictionary, "r");

    //checking whether the file loaded correctly
    if (dict == NULL)
    {
        fprintf(stderr, "Could not open dictionary.\n");
        return false;
    }

    char word[LENGTH + 1];

    //iterating over the entire dictionary
    while (fscanf(dict, "%s", word) != EOF)
    {
        //incrementing the number of words
        num++;

        //creating a new node
        node *n = malloc(sizeof(node));

        //checks if malloc succeeded
        if (n == NULL)
        {
            unload();
            return false;
        }

        //copies word into node
        strcpy(n->word, word);

        //calling the hash function to get the index of the word
        int index = hash(n->word);

        //initializes head to point to hashtable index
        node *head = table[index];

        //adding the new word into the existing linked list
        if (head == NULL)
        {
            n->next = NULL;
        }
        else
        {
            n->next = table[index];
        }
        table[index] = n;
    }


    //closing the file
    fclose(dict);

    //returning to main
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return num;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
     for (int i = 0; i < N; i++)
        {
            node *cursor = table[i];

            //unload linked list
            while (cursor != NULL)
            {
                node *temp = cursor;
                cursor = cursor->next;
                free(temp);
            }
        }
    return true;
}
