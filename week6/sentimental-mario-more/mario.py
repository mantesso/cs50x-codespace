from cs50 import get_int

input = 0
while input < 1 or input > 8:
    input = get_int("Height: ")

for i in range(input + 1):
    if i != 0:
        print((input - i) * " " + i * "#" + "  " + i * "#")