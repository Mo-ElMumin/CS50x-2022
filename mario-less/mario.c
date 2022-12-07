#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height; // get height from user
    do
    {
        height = get_int("Enter height -> : ");
    }
    while (height < 1 || height > 8); // end of do while loop
    
    for (int i = 1; i < (height + 1); i++) // for each row
    {
        for (int j = 0; j < (height - i); j++) // print spaces
        {
            printf(" ");
        }
        for (int h = 1; h < (i + 1); h++) // print #
        {
            printf("#");
        }
        printf("\n"); // go to new line
    }
}