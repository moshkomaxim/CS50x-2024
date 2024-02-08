#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool check_only_digits(string str);
char rotate_char(char ch, int key);

int main(int argc, string argv[])
{
    string text;
    int i, key;
    char ch;

    if (argc != 2 || check_only_digits(argv[1]) == false)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    key = atoi(argv[1]);
    text = get_string("plaintext: ");

    for (i = 0; text[i] != '\0'; i++)
    {
        ch = text[i];
        text[i] = rotate_char(ch, key);
    }
    text[i] = '\0';

    printf("ciphertext: %s\n", text);
}

bool check_only_digits(string str)
{
    bool check = true;
    for (int i = 0; i < strlen(str); i++)
    {
        if (isdigit(str[i]) == 0)
        {
            check = false;
        }
    }

    return check;
}

char rotate_char(char ch, int key)
{
    int num_ch, new_num_ch, base;

    if (isalpha(ch) == 0)
    {
        return ch;
    }

    num_ch = (int) ch;
    new_num_ch = num_ch + key;
    if (isupper(ch) && new_num_ch > 90)
    {
        base = 65;
        new_num_ch = new_num_ch % base;
        if (new_num_ch > 25)
        {
            new_num_ch = (new_num_ch % 26) + base;
        }
    }
    else if (islower(ch) && new_num_ch > 122)
    {
        base = 97;
        new_num_ch = new_num_ch % base;
        if (new_num_ch > 25)
        {
            new_num_ch = (new_num_ch % 26) + base;
        }
    }

    return (char) new_num_ch;
}