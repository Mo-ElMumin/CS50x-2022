# get input from user
while True:
    try:
        number = int(input("Enter number -> : "))
    except:
        print("That was not a number!")
    if number > 0:
        break
# calc length and first 2 digits
total_digits = len(str(number))
first_digits = int(number / 10 ** (total_digits - 2))
# print(first_digits)
# print(total_digits)
# checksum parameters
checksum = 0
odd = True
# calc checksum
while number != 0:
    digit = number % 10
    if odd:
        checksum += number % 10
    else:
        checksum += ((digit * 2) % 10) + int((digit * 2) / 10)
    odd = not odd
    number = int(number / 10)
# bool value for checksum result
valid = not (checksum % 10)
# print according to criteria
if valid:
    if (total_digits == 13 or total_digits == 16) and (int(first_digits / 10) == 4):
        print("VISA")
    elif (total_digits == 15) and (first_digits == 34 or first_digits == 37):
        print("AMEX")
    elif (total_digits == 16) and (first_digits > 50 and first_digits < 56):
        print("MASTERCARD")
    else:
        print("INVALID")
else:
    print("INVALID")