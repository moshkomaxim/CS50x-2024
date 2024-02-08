#include <cs50.h>
#include <stdio.h>

void print_pyramid(int height);

int main(void)
{
    int height;

    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    print_pyramid(height);

    return 0;
}

void print_pyramid(int height)
{
    int i, j, spaces;

    for (i = 1; i <= height; i++)
    {
        spaces = height - i;
        for (j = 1; j <= spaces; j++)
        {
            printf(" ");
        }
        for (j = 1; j <= height - spaces; j++)
        {
            printf("#");
        }
        printf("  ");
        for (j = 1; j <= i; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}
