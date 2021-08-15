#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    //Ensure only one arg is passed
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    FILE *photo = fopen(argv[1], "rb");
    FILE *file_n;
    //Checks if the file was opened
    if (photo == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 1;
    }
    fseek(photo, 0, SEEK_END);
    int sz = ftell(photo);
    rewind(photo);
    
    int blocks = sz / 512;
    char file_name[8];
    
    int j = 0;
    int i = 0;
    int flag = 0;
    int found = 0;
    
    //Creates an array with 512 Bytes
    BYTE *image = calloc(1, 512 * sizeof(BYTE));
    
    //Runs until the end of memory card
    while (i < blocks)
    {
        fread(image, sizeof(BYTE), 512, photo);
        if (image[0] == 0xff && image[1] == 0xd8 && image[2] == 0xff && ((image[3] & 0xf0) == 0xe0))
        {
            //If it is the first time findig a jpg heaer
            if (flag == 0)
            {
                //If is the first ten jpg use this name format
                if (j < 10)
                {
                    sprintf(file_name, "00%i.jpg", j);
                }
                //If is the after the 10 first jpg use this name format
                
                else
                {
                    sprintf(file_name, "0%i.jpg", j);
                }
                file_n = fopen(file_name, "w");
                
                if (file_n == NULL)
                {
                    fprintf(stderr, "Could not open %s.\n", argv[1]);
                    return 1;
                }
                
                flag = 1;
                found = 1;
            }
            
            else
            {
                fclose(file_n);
                
                //If is the first ten jpg usr this name
                if (j < 10)
                {
                    sprintf(file_name, "00%i.jpg", j);
                }
                
                //If is the after the 10 first jpg use this name format
                else
                {
                    sprintf(file_name, "0%i.jpg", j);
                }
                
                file_n = fopen(file_name, "w");
                
                if (file_n == NULL)
                {
                    fprintf(stderr, "Could not open %s.\n", argv[1]);
                    return 1;
                }
            }
            
            j++;
        }
       
        //If a jpg header has alredy been found recreate the image
        if (found == 1)
        {
            fwrite(image, sizeof(BYTE), 512, file_n);
            i++;
           
        }
        else
        {
            i++;
        }
    }
    fclose(file_n);
    free(image);
}
