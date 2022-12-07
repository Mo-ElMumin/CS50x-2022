// Implements a dictionary's functionality

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;
const unsigned int SIZE = 17576; // 26 ^ 3

// Hash table
node *table[SIZE];

// word counter
unsigned int words_loaded = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hash_pos = hash(word);
    node *cursor = table[hash_pos]; // point to first element in list

    while (cursor != NULL)
    {
        // compare word, if true, return true
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        // // if at the end of list, return false
        // if (cursor->next == NULL)
        // {
        //     break; // exit of loop, return false
        // }
        // else, move to next item in list
        cursor = cursor->next;
    }

    return false; // if cursor is NULL to begin with, i.e. hash_pos was empty
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // find the bucket according to the first 4 letters
    int lp = 0;
    int letter[3] = {0, 0, 0};

    int i = 0;
    // maybe better with a while loop?
    while (word[i] != '\0')
    {
        if (word[i] != '`') // if letter is not `
        {
            letter[lp] = toupper(word[i]) - 'A'; // aissign number value of valid letter array
            lp++; // move to next valid letter
        }
        if (lp > 2) // if at the end of valid letters
        {
            break; // exit for loop
        }
        i++;
    }

    int pos = letter[2] + (26 * letter[1]) + (pow(26, 2) * letter[0]); // calculate position

    // printf("Word: %s\n", word);
    // printf("    letter 1: %i \n", letter[0]);
    // printf("    letter 2: %i \n", letter[1]);
    // printf("    letter 3: %i \n", letter[2]);

    return pos;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // printf("into load \n");
    // Open files
    FILE *dict = fopen(dictionary, "r");

    if (dict == NULL)
    {
        // printf("Could not open dictionary.\n");
        return 1;
    }

    // printf("file opened \n");

    char word_buffer[LENGTH]; // buffer to store word into

    while (fscanf(dict, "%s", word_buffer) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL) // error handling
        {
            free(n); // this is needed here?
            // printf("couldnt make new node");
            return false;
        }

        strcpy(n->word, word_buffer); // copy word buffer into node.word
        n->next = NULL; // set next pointer to null

        // hash word
        int hash_pos = hash(word_buffer);

        // set new node`s next pointer to point to 1st element of list
        n->next = table[hash_pos];

        // set start of list to point to new node
        table[hash_pos] = n;

        // include word counter
        words_loaded++;

    }

    fclose(dict);

    // printf("load complete \n");

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return words_loaded;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *cursor;
    node *temp;
    int freed = 0;
    for (int i = 0; i < SIZE; i++) // for each element int he array
    {
        cursor = table[i];
        while (cursor != NULL)
        {
            temp = cursor;
            cursor = cursor->next;
            free(temp);
            freed++;
        }
    }
    // printf("%i", freed);
    return true;
}
