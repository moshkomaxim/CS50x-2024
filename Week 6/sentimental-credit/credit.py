import re
import sys


def main():
    card_number = get_number()
    card_type = check_card_type(card_number)

    if card_type == "INVALID":
        print("INVALID")
        sys.exit(1)

    checksum = calc_checksum(card_number)
    if check_checksum(checksum) is False:
        print("INVALID")
        sys.exit(1)

    print(card_type)


def get_number():
    str = input("Number: ").strip()

    return str


def check_card_type(card_number):
    AMEX_MATCH = r"^3[47]\d{13}$"
    MRCD_MATCH = r"^5[12345]\d{14}$"
    VISA_MATCH = r"^4(\d{12}|\d{15})$"

    if re.match(AMEX_MATCH, card_number) != None:
        card_type = "AMEX"
    elif re.match(MRCD_MATCH, card_number) != None:
        card_type = "MASTERCARD"
    elif re.match(VISA_MATCH, card_number) != None:
        card_type = "VISA"
    else:
        card_type = "INVALID"

    return card_type


def calc_checksum(card_number):
    card_number = card_number[::-1]
    sum1 = 0
    sum2 = 0

    for i in range(len(card_number)):
        if (i + 1) % 2 == 0:
            num = int(card_number[i]) * 2
            if num >= 10:
                num = 1 + (num % 10)
            sum1 += num
        else:
            sum2 += int(card_number[i])
    checksum = sum1 + sum2

    return checksum


def check_checksum(checksum):
    if checksum % 10 != 0:
        return False
    else:
        return True


main()
