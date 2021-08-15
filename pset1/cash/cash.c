#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    
    float change;
    do
    {
        
        change = get_float("Introduce change: ");
        
    }
    while (change < 0);
    
    int cents = round(change * 100);
    printf("%i\n", cents);
    
    // Determinates how much quarters to give and how much change is left
    int quarters = cents / 25;
    cents = cents % 25;
    
    // Determinates how much dimes to give and how much change is left
    int dimes = cents / 10;
    cents = cents % 10;

    // Determinates how much nickles to give and how much change is left
    int nickels = cents / 5;
    cents = cents % 5;

    // Determinates how much pennies to give and how much change is left
    int pennies = cents / 1;
    

    printf("You get %i coins\n", pennies + quarters + dimes + nickels);
    

}