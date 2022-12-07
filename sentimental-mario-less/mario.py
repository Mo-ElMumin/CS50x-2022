# get height from user
while True:
    try:
        height = int(input("Enter height -> : "))
        if height > 0 and height < 9:
            break
    except:
        print("not valid")

# r = row, for each Row
for r in range(1, height + 1):
    # s = space, for each prescribed space
    for s in range(height - r):
        # print that space
        print(" ", end="")
    # b = brick
    for b in range(r):
        print("#", end="")
    # print nothign to move to next line1
    print("")