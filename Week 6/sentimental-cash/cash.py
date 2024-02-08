def main():
    cents = get_cents()
    coins = get_owe(cents)
    print(f"{coins}")


def get_cents():
    cents = -1

    while cents <= 0:
        try:
            cents = float(input("Change owed: "))
        except (ValueError, TypeError):
            continue
    cents = int(cents * 100)

    return int(cents)


def get_owe(cents):
    quarters = int(cents / 25)
    cents = cents - (quarters * 25)

    dimes = int(cents / 10)
    cents = cents - (dimes * 10)

    nickels = int(cents / 5)
    cents = cents - (nickels * 5)

    pennies = cents

    coins = quarters + dimes + nickels + pennies

    return coins


main()
