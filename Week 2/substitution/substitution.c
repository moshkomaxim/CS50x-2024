#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

string get_text(void);
bool check_key(string key);
string encrypt_text(string text, string key);
void print_result(string text);

int main(int argc, string argv[])
{
    string text, key;

    if (argc != 2)
    {
        printf("Usage ./substitution key\n");
        return 1;
    }

    key = argv[1];
    if (check_key(key) == false)
        return 1;

    text = get_text();
    text = encrypt_text(text, key);
    print_result(text);
}

string get_text(void)
{
    string text = get_string("plaintext: ");
    return text;
}

bool check_key(string key)
{
    if (strlen(key) < 26 || strlen(key) > 26)
    {
        printf("Key must contain 26 characters.\n");
        return false;
    }

    for (int i = 0; i < 26; i++)
    {
        if (isalpha(key[i]) == 0)
        {
            printf("Key must contain only alphabetic characters\n");
            return false;
        }

        for (int j = 0; j < 26; j++)
        {
            if (j == i)
                continue;

            if (key[i] == key[j])
            {
                printf("Key must contain each letter exacly once\n");
                return false;
            }
        }
    }
    return true;
}

string encrypt_text(string text, string key)
{
    char ch;
    int i, index;

    for (i = 0; i < strlen(text); i++)
    {
        ch = text[i];

        if (isalpha(ch) == 0)
            continue;

        if (isupper(ch))
        {
            index = (int) ch - 65;
            ch = toupper(key[index]);
        }

        else if (islower(ch))
        {
            index = (int) ch - 97;
            ch = tolower(key[index]);
        }

        text[i] = ch;
    }

    return text;
}

void print_result(string text)
{
    printf("ciphertext: %s\n", text);
}