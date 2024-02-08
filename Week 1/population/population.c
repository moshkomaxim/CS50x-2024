#include <cs50.h>
#include <stdio.h>
int calculateYearsToGrow(int, int);
void printYears(int);

int main(void)
{
    int start_population, end_population, years_to_grow;

    do
    {
        start_population = get_int("Start size: ");
    }
    while (start_population < 9);

    do
    {
        end_population = get_int("End size: ");
    }
    while (end_population < start_population);

    years_to_grow = calculateYearsToGrow(start_population, end_population);
    printYears(years_to_grow);

    return 0;
}

int calculateYearsToGrow(int start_population, int end_population)
{
    int growth, decline;
    int years = 0;
    int population = start_population;

    while (population < end_population)
    {
        growth = population / 3;
        decline = population / 4;
        population = population + growth - decline;
        years++;
    }

    return years;
}

void printYears(int years)
{
    printf("Years: %d\n", years);
}
