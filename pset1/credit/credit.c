cd #include<stdio.h>
#include<cs50.h>
#include<math.h>

int main(void)
{
    long number;
    number = get_long("Introduce your credit card number: ");
    
    long number_div = number;
    int digits;
    int sum = 0;
    
    // Gets the number of digits od the card
    for (digits = 0; number_div > 0; digits++)
    {
        
        number_div = number_div / 10;
        
    }
    
    // Determinats the first two digits to see what type of card is 
    int two_digits = number / pow(10, (digits - 2));
    //printf("%i\n", digits);
    //printf("%i\n", two_digits);
    
    //We could use an array and would be easier.
    for (int i = 0; i <= digits / 2; i++)
    {
        // Determinate the numbers to multiply by two
        int number_2_times = number / pow(10, (2 * i + 1)) - (long)(number / pow(10, (2 * i + 2))) * 10 ;
        
        // Decompose the numbers in digits
        if (number_2_times * 2 / 10 == 0)
        {
            sum = sum + number_2_times * 2;
        }
        
        else
        {
            sum = sum + number_2_times * 2 / 10 + number_2_times * 2 % 10;
        }
        
        //Determinate the numbers not to multiply by two
        int number_not_2_times = number / pow(10, (2 * i)) - (long)(number / pow(10, (2 * i + 1))) * 10;

        sum = sum + number_not_2_times;
        
    }
    
    // Determinates if is valid and presents the right type of card
    if (digits == 15 && (two_digits == 34 ||  two_digits == 37) && sum % 10 == 0)
    {
        printf("AMEX\n");
    }        
    
    else if (digits == 16 && (two_digits >= 51 && two_digits <= 55) && sum % 10 == 0)
    {
        printf("MASTERCARD\n");
    }   
        
    else if ((digits >= 13 && digits <= 16) && (two_digits >= 40 && two_digits <= 49) && sum % 10 == 0)
    {
        printf("VISA\n");
    }   
        
    else
    {
        printf("INVALID\n");
    }
}