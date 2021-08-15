#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int s_size;
    int e_size;
    int years = 0;
    // Gets start size and makes sure it is 9 or more
    do
    {
        s_size = get_int("Introduce the start population size: ");
    }
    while (s_size < 9);
    
    // Gets end size and makes sure it is equal or more than start size
    do
    {
        e_size = get_int("Introduce the end population size: ");
    }
    while (e_size < s_size);
    
    
    // calculate number of years until it reaches end size
    for (int i = s_size; i < e_size;)
    {
        i = i + i * 1 / 3 - i * 1 / 4;
        years ++;
    }
    
    printf("Years: %i\n", years);
    
}