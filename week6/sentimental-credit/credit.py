from cs50 import get_int

card = 0

while card <= 0:
    card = get_int("Number: ")


def sum_of_digits(n):
    sum = 0
    while n > 0:
        m = n % 10
        sum += m
        n //= 10

    return sum


i, second_to_last, last = 0, 0, 0

while card:
    if i % 2 == 0:
        last += card % 10
    else:
        second_to_last += sum_of_digits(card % 10 * 2)

    if card <= 100 and card >= 10:
        first_two = card
    elif card <= 10:
        first = card

    card //= 10
    i += 1

verify_sum = int(last + second_to_last)

if verify_sum % 10 == 0:
    if (first_two == 34 or first_two == 37) and i == 15:
        print("AMEX")
    elif (
        first_two == 51
        or first_two == 52
        or first_two == 53
        or first_two == 54
        or first_two == 55
    ) and i == 16:
        print("MASTERCARD")
    elif first == 4 and (i == 13 or i == 16):
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")
