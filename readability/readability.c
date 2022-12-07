#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

int main(void)
{
    string text = get_string("Enter text here -> : "); // get input from user 

    int l = 0;  // number of letters
    int w = 1;  // number of words
    int s = 0;  // number of sentences

    for (int i = 0, j = strlen(text); i < j; i++)
    {
        if (isupper(text[i]) || islower(text[i])) // check if letter
        {
            l++;
        }
        else if (isspace(text[i])) // check if space
        {
            w++;
        }
        else if (text[i] == '?' || text[i] == '.' || text[i] == '!') // check is sentence end
        {
            s++;
        }
    }

    // calculate grade
    int index = round((0.0588 * ((float)l * 100.0 / (float)w)) - (0.296 * ((float)s * 100.0 / (float)w)) - 15.8);

    // output
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }

}