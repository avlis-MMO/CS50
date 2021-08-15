#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void substitution(string text, string KEY);

int main(int argc, string argv[])
{
    // Checks if the number os commands is right
    if (argc == 1 || argc > 2)
    {
        printf("Too few or too many command-line arguments\n");

        return 1;

    }

    //Checks if the key has exactly 26 char
    if (strlen(argv[1]) != 26)
    {
        printf("ERROR - Invalid Key\n");
        return 1;
    }
    
    
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        int count = 0;
        //Checks if the key is made only of alphabetic char and if they dont repeat
        if (!isalpha(argv[1][i]))
        {
            printf("ERROR - Invalid Key\n");
            return 1;
        }
        //Checks if char dont repeat
        for (int j = 0; j < n; j++)
        {
            if (argv[1][i] == argv[1][j])
            {
                count++;
            }

        }

        if (count > 1)
        {
            printf("ERROR - Invalid Key\n");
            return 1;
        }
    }

    //Gets the setence
    string text = get_string("Introduce your sentence: ");

    substitution(text, argv[1]);
    return 0;

}

void substitution(string plaintext, string KEY)
{
    //Creates a char array with the size of the plaintext
    char *ciphertext;
    ciphertext = (char *) malloc((strlen(plaintext) + 1) * sizeof(char));

    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        
        //checks if the letter is upper or lower case
        if (islower(plaintext[i]))
        {
            ciphertext[i] = tolower(KEY[plaintext[i] - 97]);
        }

        else if (isupper(plaintext[i]))
        {
            ciphertext[i] = toupper(KEY[plaintext[i] - 65]);
        }
        
        //Copies the non alphanumerical char
        else
        {
            ciphertext[i] = plaintext[i];
        }
    }

    printf("plaintext: %s\n", plaintext);
    printf("ciphertext: %s\n", ciphertext);
    printf("\n");


}