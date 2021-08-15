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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //Create kernels
    float Gx[3][3] = {{- 1.0, 0.0, 1.0}, {- 2.0, 0.0, 2.0}, {- 1.0, 0.0, 1.0}};
    float Gy[3][3] = {{- 1.0, - 2.0, - 1.0}, { 0.0, 0.0, 0.0}, {1.0, 2.0, 1.0}};
	
    float sum_b_x = 0.0;
    float sum_g_x = 0.0;
    float sum_r_x = 0.0;
    float sum_b_y = 0.0;
    float sum_g_y = 0.0;
    float sum_r_y = 0.0;
    float s_b = 0.0;
    float s_g = 0.0;
    float s_r = 0.0;

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
            for (int c = i - 1, x = 0; c <= i + 1; c++)
            {
                if (c < 0 || c > height - 1)
                {
                    x++;
                    continue;
                }
                else
                {
                    for (int l = j - 1, y = 0; l <= j + 1; l++)
                    {
                        if (l < 0 || l > width - 1)
                        {
                            y++;
                            continue;
                        }
                        else
                        {
                            //Sum all the pixels for Gx
                            sum_b_x = sum_b_x + image[c][l].rgbtBlue * Gx[x][y];
                            sum_g_x = sum_g_x + image[c][l].rgbtGreen * Gx[x][y];
                            sum_r_x = sum_r_x + image[c][l].rgbtRed * Gx[x][y];
                        
                            //Sum all the pixels for Gy
                            sum_b_y = sum_b_y + image[c][l].rgbtBlue * Gy[x][y];
                            sum_g_y = sum_g_y + image[c][l].rgbtGreen * Gy[x][y];
                            sum_r_y = sum_r_y + image[c][l].rgbtRed * Gy[x][y];
                        
                            y++;
                        
                        
                        }
                    }
                    x++;
                }
            }
            //Determinates the average to get the blur
            s_b = round(sqrt(pow(sum_b_x, 2.0) + pow(sum_b_y, 2.0)));
            s_g = round(sqrt(pow(sum_g_x, 2.0) + pow(sum_g_y, 2.0)));
            s_r = round(sqrt(pow(sum_r_x, 2.0) + pow(sum_r_y, 2.0)));
            
            //Caps all the values to 255
            if (s_b > 255)
            {
                s_b = 255;
            }
            
            if (s_g > 255)
            {
                s_g = 255;
            }
            
            if (s_r > 255)
            {
                s_r = 255;
            }
            
            //Creates the new image with edges
            image2[i][j].rgbtBlue = s_b;
            image2[i][j].rgbtGreen = s_g;
            image2[i][j].rgbtRed = s_r;
            
            sum_b_x = 0.0;
            sum_g_x = 0.0;
            sum_r_x = 0.0;
            sum_b_y = 0.0;
            sum_g_y = 0.0;
            sum_r_y = 0.0;
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
