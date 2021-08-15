include <stdio.h>
#include <cs50.h>

int main(void)
{
    int Height;
    
    // This will make sure the stair has more then 1 block but 8 or less
    do
    {
        Height = get_int("Introduce the height of the stairs: ");
    }
    while (Height < 1 || Height > 8);
    
    
    for (int i = 0; i < Height; i++)
    {
        
        // This loop will creat the up stair
        for (int j = 0; j < Height; j++)
        {
            if (j < Height - i - 1)
            {
                printf(" ");
            }
            
            else
            {
                printf("#");
            }
        }
        
        // Will create the space between the stairs
        printf("  ");
        
        // This loop will creat the down stair
        for (int v = 0; v < i + 1; v++)
        {
            printf("#");
        }
        
        printf("\n");
        
    }
    
    
}