#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    long long number;                            // Get Number from User
    do
    {
        number = get_long("Enter card number here -> : ");
    }
    while (number < 0);                          // end of do while loop
    long long num = number;                      // copy of number
    int checksum = 0;                            // Checksum result variable
    int total_digits = 0;                        // Digit Counter
    while (num != 0)
    {
        int digit = (num % 10);                  // extract last digit from number
        num = num / 10;                          // remove the last  digit
        total_digits += 1;                       // increase the digit counter
        if ((total_digits % 2) == true)          // if odd;
        {
            checksum += digit;                   // add odd digit to check sum
        }
        else                                     // else if even
        {
            checksum += ((digit * 2) % 10) + ((digit * 2) / 10); // double & add digits to checksum
        }
    }
    int valid = !((checksum) % 10);                             // check if checksum result is valid
    int first_digits = number / pow(10, total_digits - 2);      // trim first two digits from number
    // output
    if (valid && (total_digits == 13 || total_digits == 16) && (first_digits / 10) == 4)
    {
        printf("VISA\n");
    }
    else if (valid && (total_digits == 15) && (first_digits == 34 || first_digits == 37))
    {
        printf("AMEX\n");
    }
    else if (valid && (total_digits == 16) && (first_digits > 50 && first_digits < 56))
    {
        printf("MASTERCARD\n");
    }
    else
    {
        printf("INVALID\n");
    }
}