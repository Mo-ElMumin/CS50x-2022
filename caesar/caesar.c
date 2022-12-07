#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, string argv[])
{
    if (argc != 2) // quit if more or less than 1 additional CL arg
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    string skey = argv[1]; // save 2nd arg to as string variable
    for (int i = 0, l = strlen(skey); i < l; i++) // check each char of arg
    {
        if (!isdigit(skey[i])) // is char a digit
        {
            printf("Usage: ./caesar key\n");
            return 1; // quit if a char is not a digit
        }
    }
    int ikey = atoi(skey); // convert key to int
    string text = get_string("Enter Text to Encrypt here -> : "); // get text from user
    int factor[] = {96, 64}; // ascii factors in an Array, required for black binary magic version
    for (int i = 0, l = strlen(text); i < l; i++) // for each char in string
    {
        if (isalpha(text[i])) // if a letter
        {
            // -----------------------------
            ///// black binary magic version
            // int f = factor[0 || isupper(text[i])]; // Black Binary magic right here
            // text[i] = ((text[i] + ikey - f) % 26) + f;

            // ----------------------------------
            ///// reasonable person version
            bool upper = false;
            char c = text[i];

            // if upper case, then lower
            if (isupper(c) != 0)
            {
                upper = true;
                c = tolower(c);
            }

            // cipher
            c = c - 96; // convert to numircal position of letter in alphabet
            c = (c + ikey) % 26; // apply the cipher
            c = c + 96; // return to ascii
            // alt: c = ((c - 96 + ikey) % 26) + 96

            // if was uppercase, return to uppercase
            if (upper == true)
            {
                c = toupper(c);
            }

            // replace letter in original text
            text[i] = c; // replace letter
        }
    }
    printf("ciphertext: %s\n", text); // print out cipher
}