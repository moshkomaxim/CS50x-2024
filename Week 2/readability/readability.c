#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

int calc_grade(string);
void print_grade(int);

int main(void)
{
    int grade;
    string text;

    text = get_string("Text: ");
    grade = calc_grade(text);
    print_grade(grade);
}

int calc_grade(string text)
{
    // Congratulations! Today is your day. You're off to Great Places! You're off and away!
    // The text the user inputted has 65 letters, 4 sentences, and 14 words. 65 letters per 14 words
    int letters = 0, words = 0, sentences = 0;
    float grade;
    float L, S;
    bool start_word;

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
            if (text[i - 1] == ' ')
            {
                start_word = true;
            }
        }
        else if (isspace(text[i]))
        {
            words++;
            start_word = false;
        }
        else if (text[i] == '!' || text[i] == '?' || text[i] == '.')
        {
            sentences++;
        }
    }
    words++;

    L = (float) letters / words * 100;
    S = (float) sentences / words * 100;
    grade = 0.0588 * L - 0.296 * S - 15.8;

    return round(grade);
}

void print_grade(int grade)
{
    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %d\n", grade);
    }
}