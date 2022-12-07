#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    if (score1 == score2) // if equal scores
    {
        printf("Tie!");
    }
    else if (score1 > score2)
    {
        printf("player 1 wins!");
    }
    else
    {
        printf("player 2 wins!");
    }
}

int compute_score(string word)
{
    int points = 0;
    for (int i = 0, l = strlen(word); i < l; i++) // for each char in word
    {
        int value = word[i];
        if (isalpha(value) && isupper(value)) // if alphabetical and upper case
        {
            value = tolower(value); // make lower
        }
        if (value >= 97 && value <= 122) // if between a and Z
        {
            points += POINTS[value - 97]; // 
        }
    }
    return points;
}
