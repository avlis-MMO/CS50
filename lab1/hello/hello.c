#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Gets the name
    string name = get_string("Input your name: ");
    
    printf("Hello, %s \n", name);
    
}