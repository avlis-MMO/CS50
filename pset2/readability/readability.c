#include<stdio.h>
#include<cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//Prototype
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Input your text: ");
    
    // Counts the required information
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);
    
    //Calculate the values of L and S used in the function given
    float L = letters * 100.0 / words;
    float S = sentences * 100.0 / words;
    
    int Grade = round(0.0588 * L - 0.296 * S - 15.8);
    
    //Prints the grade in the format asked
    if (Grade >= 16)
    {
        printf("Grade 16+\n");
    }
    
    else if (Grade < 1)
    {
        printf("Before Grade 1\n");
    }
    
    else
    {
        printf("Grade %i\n", Grade);
    }
}

int count_letters(string text)
{
    
    int number_letters = 0;
    
    //Counts all the letters in the text
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        //Checks if it is a letter or not
        if (isalpha(text[i]))
        {
            number_letters ++; 
        }
        
    }
    
    return number_letters;
    
}

int count_words(string text)
{
    
    int number_words = 0;
    
    //Counts all the words in the text
    for (int i = 0, n = strlen(text) + 1; i < n; i++)
    {
        //Checks if it is a space or end of text
        if (text[i] == ' ' || text[i] == '\0')
        {
            number_words ++; 
        }
        
    }
    
    return number_words;
    
}

int count_sentences(string text)
{
    int number_sentences = 0;
    
    //Counts all the sentnces in the text
    for (int i = 0, n = strlen(text) + 1; i < n; i++)
    {
        //Checks if it is '.', '?', '!'
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            number_sentences ++; 
        }
        
    }
    
    return number_sentences;
    
    
}