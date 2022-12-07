from cs50 import get_float

while True:
    try:
        dollars = get_float("Enter cents -> : ")
    except:
        print("That was not a number!")
    if dollars > 0:
        break

# convert to cents
cents = int(dollars * 100.0)
print(cents)
# calc quarters
quarters = int(cents / 25)
cents = cents - quarters * 25
# calc dimes
dimes = int(cents / 10)
cents = cents - dimes * 10
# calc nickels
nickels = int(cents / 5)
cents = cents - nickels * 5
# calc pennies
pennies = int(cents)
# calc total coins
coins = quarters + dimes + nickels + pennies
# print total coins
print(coins)