#include "helpers.h"
#include "math.h"
#include "cs50.h"
#include "stdio.h"

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
            BYTE *right[3] = {&image[i][width - j - 1].rgbtRed, &image[i][width - j - 1].rgbtGreen, &image[i][width - j - 1].rgbtBlue};

            for (int c = 0; c < 3; c++)
            {
                *right[c] = *left[c];
            }
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE edge[height][width];

    int gx_grid[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy_grid[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++) // for each row
    {
        for (int j = 0; j < width; j++) /// for each column
        {
            // for each pixel at x = i, y = j
            int gx_red = 0;
            int gx_green = 0;
            int gx_blue = 0;

            int gy_red = 0;
            int gy_green = 0;
            int gy_blue = 0;

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
                        int gx_v = gx_grid[y + 1][x + 1];
                        gx_red += (int) image[i + y][j + x].rgbtRed * gx_v;
                        gx_green += (int) image[i + y][j + x].rgbtGreen * gx_v;
                        gx_blue += (int) image[i + y][j + x].rgbtBlue * gx_v;

                        int gy_v = gy_grid[y + 1][x + 1];
                        gy_red += (int) image[i + y][j + x].rgbtRed * gy_v;
                        gy_green += (int) image[i + y][j + x].rgbtGreen * gy_v;
                        gy_blue += (int) image[i + y][j + x].rgbtBlue * gy_v;
                    }
                }
            }

            // calculate the ####

            int sq_red = pow(gx_red, 2) + pow(gy_red, 2);
            int sq_green = pow(gx_green, 2) + pow(gy_green, 2);
            int sq_blue = pow(gx_blue, 2) + pow(gy_blue, 2);

            float sr_red = sqrt(sq_red);
            float sr_green = sqrt(sq_green);
            float sr_blue = sqrt(sq_blue);

            int new_red = round(sr_red);
            int new_green = round(sr_green);
            int new_blue = round(sr_blue);

            // pop values into pointer array
            int *rgb[3] = {&new_red, &new_green, &new_blue};

            // check if any value is > 255 or < 0
            for (int c = 0; c < 3; c++)
            {
                if (*rgb[c] > 255)
                {
                    *rgb[c] = 255; // set max
                }
                else if (*rgb[c] < 0)
                {
                    *rgb[c] = 0; // set min
                }
            }

            // put into temp array
            edge[i][j].rgbtRed = new_red;
            edge[i][j].rgbtGreen = new_green;
            edge[i][j].rgbtBlue = new_blue;
        }
    }

    for (int i = 0; i < height; i++) // for every row
    {
        for (int j = 0; j < width; j++) // for every column
        {
            // replace everything with new edge values
            image[i][j].rgbtRed = edge[i][j].rgbtRed;
            image[i][j].rgbtGreen = edge[i][j].rgbtGreen;
            image[i][j].rgbtBlue = edge[i][j].rgbtBlue;
        }
    }

    return;
}
