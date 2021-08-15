#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j ++)
        {
            //Gets the RGB values of the pixels
            int b = image[i][j].rgbtBlue;
            int g = image[i][j].rgbtGreen;
            int r = image[i][j].rgbtRed;
            
            //Gets the average of RGB values
            float average = round((b + g + r) / 3.0);
            
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
        
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j ++)
        {
            //Gets the RGB values of the pixels
            int b = image[i][j].rgbtBlue;
            int g = image[i][j].rgbtGreen;
            int r = image[i][j].rgbtRed;
            
            //Applies the formula to obtain the new RGB values
            float sepiaRed = round(.393 * r + .769 * g + .189 * b);
            float sepiaGreen = round(.349 * r + .686 * g + .168 * b);
            float sepiaBlue = round(.272 * r + .534 * g + .131 * b);
            
            //Makes sure the value isnt higher than 255 if it is caps at 255
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            
            //Makes sure the value isnt higher than 255 if it is caps at 255
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            
            //Makes sure the value isnt higher than 255 if it is caps at 255
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }
            
            image[i][j].rgbtBlue = sepiaBlue;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtRed = sepiaRed;
        }
        
    }
    
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //Create a temporary variable
    RGBTRIPLE *aux;
    aux = malloc(sizeof(RGBTRIPLE));
    int start = 0;
    int w = width;
    
    if (aux == NULL)
    {
        fprintf(stderr, "Couldn´t create variable.\n");
    }
    
    //Reflects the pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < w - 1; j++, w--)
        {
            
            *aux = image[i][j];
            image[i][j] = image[i][w - 1];
            image[i][w - 1] = *aux;
        }
        w = width;
    }
    free(aux);
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int sum_b = 0;
    int sum_g = 0;
    int sum_r = 0;
    float count = 0.0;

    //Creates a new iamge to store temporary the changed pixels
    RGBTRIPLE(*image2)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image2 == NULL)
    {
        fprintf(stderr, "Not enough memory to store image.\n");
    }
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j ++)
        {
            //Go through the pixels around the pixel we want to blur
            for (int c = i - 1; c <= i + 1; c++)
            {
                if (c < 0 || c > height - 1)
                {
                    continue;
                }
                else
                {
                    for (int l = j - 1; l <= j + 1; l++)
                    {
                        if (l < 0 || l > width - 1)
                        {
                            continue;
                        }
                        else
                        {
                            sum_b = sum_b + image[c][l].rgbtBlue;
                            sum_g = sum_g + image[c][l].rgbtGreen;
                            sum_r = sum_r + image[c][l].rgbtRed;
                            count ++;
                        }
                    }
                }
            }
            //Determinates the average to get the blur
            image2[i][j].rgbtBlue = round(sum_b / count);
            image2[i][j].rgbtGreen = round(sum_g / count);
            image2[i][j].rgbtRed = round(sum_r / count);
            
            sum_b = 0;
            sum_g = 0;
            sum_r = 0;
            count = 0.0;
        }
    }
    
    //Transfer the aux array to the original array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j ++)
        {
            //Gets the RGB values of the pixels
            image[i][j] = image2[i][j];
        }
        
    }
    return;
}
