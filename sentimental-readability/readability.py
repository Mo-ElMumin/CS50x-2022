from cs50 import get_string

# get input from user
text = get_string("Enter text here -> : ")

# initalise counting variables
l = 0
w = 1
s = 0

# parse text
for c in text:
    if c.isalpha():
        l += 1
    elif c == " ":
        w += 1
    elif c == "?" or c == "." or c == "!":
        s += 1

# calc index
index = round((0.0588 * l * 100 / w) - (0.296 * s * 100 / w) - 15.8)

# print output
if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print("Grade " + str(index))
