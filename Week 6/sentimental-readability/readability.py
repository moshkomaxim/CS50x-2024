def main():
    text = get_text()
    grade = calc_grade(text)
    print_grade(grade)


def get_text():
    return input("Text: ").strip()


def calc_grade(text):
    letters = 0
    words = 0
    sentences = 0
    start_word = False

    for ch in text:
        if ch.isalpha():
            letters += 1
            start_word = True
        elif ch.isspace():
            words += 1
            start_word = False
        elif ch in ["!", "?", "."]:
            sentences += 1
    words += 1

    L = float(letters) / words * 100
    S = float(sentences) / words * 100
    grade = 0.0588 * L - 0.296 * S - 15.8

    return round(grade)


def print_grade(grade):
    if grade >= 16:
        print("Grade 16+")
    elif grade < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {grade}")


main()
