// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
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

    uint8_t header[HEADER_SIZE]; // header buffer
    fread(header, HEADER_SIZE, 1, input); // read header, save into header
    fwrite(header, HEADER_SIZE, 1, output); // read header, save into output

    int16_t buffer; // sample buffer
    while (fread(&buffer, sizeof(int16_t), 1, input)) // while reading from memory
    {
        buffer = buffer * factor; // modify data
        fwrite(&buffer, sizeof(int16_t), 1, output); // write to output file
    } // continues while there more left to read
    
    // Close files
    fclose(input);
    fclose(output);
}
