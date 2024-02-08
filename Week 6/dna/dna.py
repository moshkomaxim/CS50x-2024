import csv
import sys


def main():
    database = []

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")
    if sys.argv[1].endswith(".csv") == False:
        sys.exit("Usage: python dna.py data.csv sequence.txt")
    if sys.argv[2].endswith(".txt") == False:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # TODO: Read database file into a variable
    with open(sys.argv[1]) as file:
        reader = csv.DictReader(file)
        headers = reader.fieldnames

        for line in reader:
            person = {}
            for header in headers:
                person.update({header: line[header]})
            database.append(person)
        headers.pop(0)

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2]) as file:
        for line in file:
            sequence = line

    # TODO: Find longest match of each STR in DNA sequence
    longest_headers = []
    for header in headers:
        lenght = longest_match(sequence, header)
        longest_headers.append({header: lenght})

    # TODO: Check database for matching profiles
    matches = 0
    for line in database:
        for header, long_header in zip(headers, longest_headers):
            if long_header[header] == int(line[header]):
                matches += 1
                if matches == len(headers):
                    print(line["name"])
                    return
        matches = 0

    print("No match")


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
