#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    if (argc != 2) // quit if more or less than 1 additional CL arg
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    int key_length = strlen(argv[1]); // check length of key
    if (key_length != 26)
    {
        printf("Key must be 26 letters long\n");
        return 1;
    }
    // check all chars are letters
    string skey = argv[1]; // save 2nd arg to as string variable
    for (int i = 0; i < key_length; i++) // check each char of arg
    {
        if (!isalpha(skey[i])) // is char a digit
        {
            printf("Non-Alphabetical char in key!\n");
            return 1; // quit cos char is not a letter
        }
    }
    int legend[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // shift legend array
    int duplic[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // duplicate array
    int factor[] = {96, 64};
    for (int i = 0; i < key_length; i++)
    {
        int f = factor[0 || isupper(skey[i])];
        if (duplic[skey[i] - f] != '\0')
        {
            printf("Duplicate at pos %i? \n", i);
            return 1;
        }
        duplic[skey[i] - f] = 1;
        legend[i] = (skey[i] - f) - i - 1;
    }
    string text = get_string("plaintext:"); // get text from user
    for (int i = 0, l = strlen(text); i < l; i++)
    {
        if (isalpha(text[i])) // if a letter
        {
            int f = factor[0 || isupper(text[i])];      // Black Binary magic right here again
            text[i] = text[i] + legend[text[i] - f - 1]; // change to letter + sub_shift
        }
    }
    printf("ciphertext: %s\n", text); // print out cipher
}