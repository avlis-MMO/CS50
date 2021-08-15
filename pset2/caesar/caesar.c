#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void caesar(string str, int n);

string alphabet = "abcdefghijklmnopqrstuvwxyz";

int main(int argc, char **argv)
{
    
    // Checks if the number os commands is right
    if (argc == 1 || argc > 2)
    {
        printf("Too few or too many command-line arguments\n");

        return 1;
    }

    //Checks if the key is a digit positive
    if (atoi(argv[1]) < 1)
    {
        printf("ERROR - Invalid Key\n");
        return 1;
    }
    
    //Sees if is a digit only
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
        
    }
    
    string text = get_string("Introduce your sentence: ");
    
    caesar(text, atoi(argv[1]));
 
    return 0;   
}

void caesar(string plaintext, int KEY)
{
    
    //Creates an empty array of char
    char ciphertext[strlen(plaintext)];
    //ciphertext = (char *) malloc((strlen(plaintext) + 1) * sizeof(char));
    memset(ciphertext, 0, strlen(plaintext));
    
    //Determinates how many time KEY is bigger than 26
    int mult = KEY / 26; 
    //Determinates how much the KEY is after having in acount that it can go around. Exemp. KEY = 27 A -> B 
    KEY = KEY - mult * 26;
    

    
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        //checks if the letter is upper or lower case
        if (islower(plaintext[i]))
        {
            ciphertext[i] = alphabet[((int)(plaintext[i]) - 97 + KEY) % 26];
        }
        
        else if (isupper(plaintext[i]))
        {
            ciphertext[i] = toupper(alphabet[((int)(plaintext[i]) - 65 + KEY) % 26]);
        }
        
        //Copies the non alphanumerical char
        else
        {
            ciphertext[i] = plaintext[i];
        }
    }
    
    printf("plaintext: %s\n", plaintext);
    printf("ciphertext: %s\n", ciphertext);
}