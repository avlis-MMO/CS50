// Modifies the volume of an audio file

#include <cs50.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;
typedef uint8_t BYTES;
typedef int16_t BUFFER;

int main(int argc, char *argv[])
{
    BYTES header[HEADER_SIZE];
    BUFFER buffer;
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);
    
    
    // Copies the whole header file since we know the size
    fread(header, sizeof(BYTES), 44, input);
    fwrite(header, sizeof(BYTES), 44, output);

    // Reads 2 bytes a time and multplies the value by the factor
    while (fread(&buffer, sizeof(BUFFER), 1, input))
    {
        buffer = buffer * factor;
        fwrite(&buffer, sizeof(BUFFER), 1, output);
    }
    // Close files
    fclose(input);
    fclose(output);
}
