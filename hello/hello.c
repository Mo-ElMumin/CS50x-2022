#include <stdio.h>
#include <cs50.h>

int main(void)
{
    string name = get_string("Enter name here -> : "); // prompt user for name
    printf("Hello, %s, Welcome to VS code! \n", name); // print name onto screen
}