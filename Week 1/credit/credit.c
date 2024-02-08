#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long card_number;
    int card_digits[16];
    int i, num, checksum, card_len, first, second;
    int sum_1 = 0, sum_2 = 0;
    string card_type;

    // Get card number and reprompt if needed
    do
    {
        card_number = get_long("Number: ");
    }
    while (card_number < 9);

    card_len = 0;
    // Extract digits from card_number and place them in array
    for (i = 0; card_number != 0; i++)
    {
        card_digits[i] = card_number % 10;
        card_number /= 10;
        card_len += 1;
    }

    // Checking card prefix
    first = card_digits[card_len - 1];
    second = card_digits[card_len - 2];

    if (first == 3 && (second == 4 || second == 7))
    {
        card_type = "AMEX";
    }
    else if (first == 5 && (second == 1 || second == 2 || second == 3 || second == 4 || second == 5))
    {
        card_type = "MASTERCARD";
    }
    else if (first == 4)
    {
        card_type = "VISA";
    }
    else
    {
        card_type = "INVALID";
    }

    // Check card lenght
    if (card_type[0] == 'A' && card_len == 15)
    {
        ;
    }
    else if (card_type[0] == 'M' && card_len == 16)
    {
        ;
    }
    else if (card_type[0] == 'V' && (card_len == 13 || card_len == 16))
    {
        ;
    }
    else
    {
        card_type = "INVALID";
    }

    // Calculating checksum
    for (i = 0; i < 16; i++)
    {
        int sum1, sum2;

        if ((i + 1) % 2 == 0)
        {
            num = card_digits[i] * 2;
            if (num >= 10)
            {
                num = 1 + (num % 10);
            }
            sum_1 += num;
        }
        else
        {
            sum_2 += card_digits[i];
        }
    }
    checksum = sum_1 + sum_2;

    // Check checksum
    if (checksum % 10 != 0)
    {
        card_type = "INVALID";
    }

    printf("%s\n", card_type);
}