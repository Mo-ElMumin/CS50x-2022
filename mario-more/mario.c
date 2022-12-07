#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Enter height -> : "); // get height from user
    }
    while (height < 1 || height > 8);
    for (int i = 1; i < (height + 1); i++)  // for each row
    {
        for (int j = 0; j < (height - i); j++) // print spaces
        {
            printf(" ");
        }
        for (int h = 1; h < (i + 1); h++) // print ##
        {
            printf("#");
        }
        printf("  "); // print gap inbetween
        for (int k = 0; k < i; k++) // print second set of #
        {
            printf("#");
        }
        printf("\n"); // go to next line
    }
}