# get user input
while True:
    try:
        height = int(input("Enter height -> : "))
        if height > 0 and height < 9:
            break
    except:
        print("not valid")

# for each row, r = row
for r in range(height):
    # print spaces, s = spaces
    for s in range(1, height - r):
        print(" ", end="")
    # b = bricks
    for b in range(r + 1):
        print("#", end="")
    # print gaps
    print("  ", end="")
    # print second set of bricks
    for b in range(r + 1):
        print("#", end="")
    # print nothing to go to new line
    print("")
