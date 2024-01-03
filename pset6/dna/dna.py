import csv
import sys


def main():
    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py [DATABASES] [SEQUENCES]")
        sys.exit(1)

    db_file = sys.argv[1]
    seq_file = sys.argv[2]

    # Read database file into a variable
    with open(db_file, "r") as data:
        reader = csv.DictReader(data)
        databases = []
        strs = reader.fieldnames

        for row in reader:
            databases.append(row)

    strs.remove("name")

    # Read DNA sequence file into a variable
    with open(seq_file, "r") as file:
        sequence = file.read().strip()

    # Find longest match of each STR in DNA sequence
    check = {}
    for str_sequence in strs:
        check[str_sequence] = longest_match(sequence, str_sequence)

    # Check database for matching profiles
    for person in databases:
        if check_person(person, check):
            print(person["name"])
            return

    print("No match")
    return


def check_person(person, check):
    for str_sequence in check:
        if int(person[str_sequence]) != check[str_sequence]:
            return False

    return True


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):
        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:
            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
