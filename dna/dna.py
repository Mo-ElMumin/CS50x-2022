import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        # sys.argv[0] = dna.py
        # sys.argv[1] = database.csv
        # sys.argv[2] = sequence.txt
        sys.exit("Usage: python dna.py Database.csv Sequence.txt")

    # Variable to store database as a list
    people = []
    str = []

    # Read database file into a variable
    with open(sys.argv[1], "r") as database:
        # Extract Str from top line
        str_reader = csv.reader(database)
        str = next(str_reader)
        str.remove("name")
        # reset reader back to beginning
        database.seek(0)
        # extraction dictionaries;
        reader = csv.DictReader(database)
        for row in reader:
            for i in str:
                # convert counts from strings to int
                row[i] = int(row[i])
            # add to list of people
            people.append(row)

    # Read DNA sequence file into a variable
    seq_file = open(sys.argv[2], "r")
    sequence = seq_file.read()

    # Find longest match of each STR in DNA sequence
    suspect = {}
    for i in str:
        suspect[i] = longest_match(sequence, i)

    # Check database for matching profiles
    # for each person
    for person in people:
        invalid = False
        # for each str
        for i in str:
            # compare every str,
            if person[i] != suspect[i]:
                # if any are found to not be a match, set as invalid and break
                invalid = True
                break
        # if invalid skip to next person
        if invalid == True:
            continue
        # only runs if a person != invalid, i.e. they are a vlid match
        # print name and exit program
        print(person["name"])
        return
    # if no match is found, print no match and quit
    print("No match")
    return


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
