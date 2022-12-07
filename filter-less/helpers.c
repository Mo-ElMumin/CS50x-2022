#include "helpers.h"
#include "math.h"
#include "stdio.h"
#include "cs50.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++) // for each row
    {
        for (int j = 0; j < width; j++) // for each column
        {
            // rgbtBlue
            float sum = (float)image[i][j].rgbtRed + (float)image[i][j].rgbtGreen + (float)image[i][j].rgbtBlue; // sum all values
            int average = round(sum / 3.0); // calculate averages
            // set all values to average
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++) // for every row
    {
        for (int j = 0; j < width; j++) // for every column
        {
            // apply sepia formula
            int s_red = round((0.393 * (float)image[i][j].rgbtRed) + (0.769 * (float)image[i][j].rgbtGreen) + (0.189 *
                              (float)image[i][j].rgbtBlue));
            int s_green = round((0.349 * (float)image[i][j].rgbtRed) + (0.686 * (float)image[i][j].rgbtGreen) + (0.168 *
                                (float)image[i][j].rgbtBlue));
            int s_blue = round((0.272 * (float)image[i][j].rgbtRed) + (0.534 * (float)image[i][j].rgbtGreen) + (0.131 *
                               (float)image[i][j].rgbtBlue));

            int *rgb[3] = {&s_red, &s_green, &s_blue};

            for (int c = 0; c < 3; c++) // for each colours
            {
                if (*rgb[c] > 255) // if > max, set to max
                {
                    *rgb[c] = 255;
                }
                else if (*rgb[c] < 0) // if < min, set to min
                {
                    *rgb[c] = 0;
                }
            }
            // apply to image
            image[i][j].rgbtRed = s_red;
            image[i][j].rgbtGreen = s_green;
            image[i][j].rgbtBlue = s_blue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++) // for each row
    {
        for (int j = 0; j < width; j++) /// for each column
        {

            if (j >= width / 2) // if greater than half way through the picture
            {
                break; // stop
            }

            // swap using pointer magic

            BYTE temp[3] = {image[i][width - j - 1].rgbtRed, image[i][width - j - 1].rgbtGreen, image[i][width - j - 1].rgbtBlue};
            BYTE *left[3] = {&image[i][j].rgbtRed, &image[i][j].rgbtGreen, &image[i][j].rgbtBlue};

            image[i][width - j - 1].rgbtRed = image[i][j].rgbtRed;
            image[i][width - j - 1].rgbtGreen = image[i][j].rgbtGreen;
            image[i][width - j - 1].rgbtBlue = image[i][j].rgbtBlue;

            for (int c = 0; c < 3; c++)
            {
                *left[c] = temp[c];
            }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blur[height][width];

    for (int i = 0; i < height; i++) // for each row
    {
        for (int j = 0; j < width; j++) /// for each column
        {
            // for each pixel at x = i, y = j
            int s_red = 0;
            int s_green = 0;
            int s_blue = 0;
            int box_counter = 0;

            //// Travel around pixel
            for (int y = -1; y < 2; y++)
            {
                for (int x = -1; x < 2; x++)
                {
                    bool w_invalid = ((j + x < 0) || (j + x >= width));
                    bool h_invalid = ((i + y < 0) || (i + y >= height));
                    bool valid = !(w_invalid || h_invalid);

                    if (valid) // if valid add colour value to total sum & increase counter
                    {
                        box_counter++;
                        s_red += image[i + y][j + x].rgbtRed;
                        s_green += image[i + y][j + x].rgbtGreen;
                        s_blue += image[i + y][j + x].rgbtBlue;
                    }
                }
            }

            // calculate avergae and set value in temp box
            blur[i][j].rgbtRed = round((float) s_red / (float) box_counter);
            blur[i][j].rgbtGreen = round((float) s_green / (float) box_counter);
            blur[i][j].rgbtBlue = round((float) s_blue / (float) box_counter);
        }
    }

    for (int i = 0; i < height; i++) // for every row
    {
        for (int j = 0; j < width; j++) // for every column
        {
            // replace everything with blur value
            image[i][j].rgbtRed = blur[i][j].rgbtRed;
            image[i][j].rgbtGreen = blur[i][j].rgbtGreen;
            image[i][j].rgbtBlue = blur[i][j].rgbtBlue;
        }
    }

    return;
}
