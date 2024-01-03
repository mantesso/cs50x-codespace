from cs50 import get_string

text = get_string("Text: ")


def count_letters(text):
    count = 0
    for char in text:
        if char.isalpha():
            count += 1
    return count


def count_words(text):
    count = 0
    for char in text:
        if char.isspace():
            count += 1
    return count + 1


def count_sentences(text):
    count = 0
    for char in text:
        if char in [".", "!", "?"]:
            count += 1
    return count


def coleman_index(text):
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    L = letters / words * 100
    S = sentences / words * 100

    return 0.0588 * L - 0.296 * S - 15.8


def grade(coleman):
    if coleman < 1:
        print("Before Grade 1")
    elif coleman > 16:
        print("Grade 16+")
    else:
        print(f"Grade {round(coleman)}")


grade(coleman_index(text))
