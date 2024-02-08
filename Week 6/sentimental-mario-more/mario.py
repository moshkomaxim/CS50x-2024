def main():
    height = 0
    while height < 1 or height > 8:
        try:
            height = int(input("Height: ").strip())
        except (TypeError, ValueError):
            continue
    print_pyramid(height)


def print_pyramid(height):
    for i in range(height):
        spaces = " " * (height - i - 1)
        hashes = "#" * (i + 1)
        spaces_2 = "  "
        hashes_2 = "#" * (i + 1)
        print(spaces + hashes + spaces_2 + hashes_2)


main()
