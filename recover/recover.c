#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE;
const int block_size = 512;

FILE *open_new_jpeg(int count);

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open files
    FILE *raw = fopen(argv[1], "r");
    if (raw == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // read blocks
    BYTE bb[block_size];
    int file_counter = 0;
    FILE *current_image;
    current_image = open_new_jpeg(file_counter);

    while (fread(bb, sizeof(BYTE), block_size, raw)) // while reading Data
    {
        // check for jpeg header
        bool new_jpeg = (bb[0] == 0xff) && (bb[1] == 0xd8) && (bb[2] = 0xff) && ((bb[3] & 0xf0) == 0xe0);

        if (new_jpeg)
        {
            if (file_counter != 0) // if not first pass
            {
                fclose(current_image);
                current_image = open_new_jpeg(file_counter);
            }
            fwrite(bb, sizeof(BYTE), block_size, current_image);
            file_counter++;
        }
        // if not new file and not first pass 
        else if (!new_jpeg && file_counter != 0)
        {
            fwrite(bb, sizeof(BYTE), block_size, current_image);
        }
    }

    // final clean up
    fclose(current_image);
    fclose(raw);
}

FILE *open_new_jpeg(int count)
{
    char filename[15];
    sprintf(filename, "%03i.jpg", count);
    FILE *jpeg = fopen(filename, "w");
    return jpeg;
}
