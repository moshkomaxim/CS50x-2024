#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    short average;
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            average = round((image[h][w].rgbtRed + image[h][w].rgbtBlue + image[h][w].rgbtGreen) / 3.0);
            {
                image[h][w].rgbtRed = average;
                image[h][w].rgbtBlue = average;
                image[h][w].rgbtGreen = average;
            }
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    short sepiaRed, sepiaGreen, sepiaBlue;
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            {
                sepiaRed = round(.393 * image[h][w].rgbtRed + .769 * image[h][w].rgbtGreen + .189 * image[h][w].rgbtBlue);
                sepiaGreen = round(.349 * image[h][w].rgbtRed + .686 * image[h][w].rgbtGreen + .168 * image[h][w].rgbtBlue);
                sepiaBlue = round(.272 * image[h][w].rgbtRed + .534 * image[h][w].rgbtGreen + .131 * image[h][w].rgbtBlue);

                if (sepiaRed > 255)
                    sepiaRed = 255;
                if (sepiaGreen > 255)
                    sepiaGreen = 255;
                if (sepiaBlue > 255)
                    sepiaBlue = 255;

                image[h][w].rgbtRed = sepiaRed;
                image[h][w].rgbtBlue = sepiaBlue;
                image[h][w].rgbtGreen = sepiaGreen;
            }
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Make a copy
    RGBTRIPLE copy_image[height][width];
    short reflect_j;

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            copy_image[h][w].rgbtRed = image[h][w].rgbtRed;
            copy_image[h][w].rgbtBlue = image[h][w].rgbtBlue;
            copy_image[h][w].rgbtGreen = image[h][w].rgbtGreen;
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            reflect_j = (width - 1) - w;
            image[h][reflect_j].rgbtRed = copy_image[h][w].rgbtRed;
            image[h][reflect_j].rgbtBlue = copy_image[h][w].rgbtBlue;
            image[h][reflect_j].rgbtGreen = copy_image[h][w].rgbtGreen;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Make a copy of image
    RGBTRIPLE copy_image[height][width];
    float blur_red, blur_blue, blur_green;

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            copy_image[h][w].rgbtRed = image[h][w].rgbtRed;
            copy_image[h][w].rgbtBlue = image[h][w].rgbtBlue;
            copy_image[h][w].rgbtGreen = image[h][w].rgbtGreen;
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {

            // Set blur for corners
            if (h == 0 && w == 0)
            {
                blur_red = (copy_image[h][w].rgbtRed + copy_image[h][w + 1].rgbtRed + copy_image[h + 1][w].rgbtRed +
                            copy_image[h + 1][w + 1].rgbtRed) /
                           4.0;
                blur_blue = (copy_image[h][w].rgbtBlue + copy_image[h][w + 1].rgbtBlue + copy_image[h + 1][w].rgbtBlue +
                             copy_image[h + 1][w + 1].rgbtBlue) /
                            4.0;
                blur_green = (copy_image[h][w].rgbtGreen + copy_image[h][w + 1].rgbtGreen + copy_image[h + 1][w].rgbtGreen +
                              copy_image[h + 1][w + 1].rgbtGreen) /
                             4.0;
            }
            else if (h == 0 && w == (width - 1))
            {
                blur_red = (copy_image[h][w - 1].rgbtRed + copy_image[h][w].rgbtRed + copy_image[h + 1][w - 1].rgbtRed +
                            copy_image[h + 1][w].rgbtRed) /
                           4.0;
                blur_blue = (copy_image[h][w - 1].rgbtBlue + copy_image[h][w].rgbtBlue + copy_image[h + 1][w - 1].rgbtBlue +
                             copy_image[h + 1][w].rgbtBlue) /
                            4.0;
                blur_green = (copy_image[h][w - 1].rgbtGreen + copy_image[h][w].rgbtGreen + copy_image[h + 1][w - 1].rgbtGreen +
                              copy_image[h + 1][w].rgbtGreen) /
                             4.0;
            }
            else if (h == (height - 1) && w == 0)
            {
                blur_red = (copy_image[h][w].rgbtRed + copy_image[h][w + 1].rgbtRed + copy_image[h - 1][w].rgbtRed +
                            copy_image[h - 1][w + 1].rgbtRed) /
                           4.0;
                blur_blue = (copy_image[h][w].rgbtBlue + copy_image[h][w + 1].rgbtBlue + copy_image[h - 1][w].rgbtBlue +
                             copy_image[h - 1][w + 1].rgbtBlue) /
                            4.0;
                blur_green = (copy_image[h][w].rgbtGreen + copy_image[h][w + 1].rgbtGreen + copy_image[h - 1][w].rgbtGreen +
                              copy_image[h - 1][w + 1].rgbtGreen) /
                             4.0;
            }
            else if (h == (height - 1) && w == (width - 1))
            {
                blur_red = (copy_image[h][w - 1].rgbtRed + copy_image[h][w].rgbtRed + copy_image[h - 1][w - 1].rgbtRed +
                            copy_image[h - 1][w].rgbtRed) /
                           4.0;
                blur_blue = (copy_image[h][w - 1].rgbtBlue + copy_image[h][w].rgbtBlue + copy_image[h - 1][w - 1].rgbtBlue +
                             copy_image[h - 1][w].rgbtBlue) /
                            4.0;
                blur_green = (copy_image[h][w - 1].rgbtGreen + copy_image[h][w].rgbtGreen + copy_image[h - 1][w - 1].rgbtGreen +
                              copy_image[h - 1][w].rgbtGreen) /
                             4.0;
            }

            // Set blur for sides
            else if (h == 0 && w < width)
            {
                blur_red = (copy_image[h][w - 1].rgbtRed + copy_image[h][w].rgbtRed + copy_image[h][w + 1].rgbtRed +
                            copy_image[h + 1][w - 1].rgbtRed + copy_image[h + 1][w].rgbtRed + copy_image[h + 1][w + 1].rgbtRed) /
                           6.0;
                blur_blue =
                    (copy_image[h][w - 1].rgbtBlue + copy_image[h][w].rgbtBlue + copy_image[h][w + 1].rgbtBlue +
                     copy_image[h + 1][w - 1].rgbtBlue + copy_image[h + 1][w].rgbtBlue + copy_image[h + 1][w + 1].rgbtBlue) /
                    6.0;
                blur_green =
                    (copy_image[h][w - 1].rgbtGreen + copy_image[h][w].rgbtGreen + copy_image[h][w + 1].rgbtGreen +
                     copy_image[h + 1][w - 1].rgbtGreen + copy_image[h + 1][w].rgbtGreen + copy_image[h + 1][w + 1].rgbtGreen) /
                    6.0;
            }
            else if (h == (height - 1) && w < width)
            {
                blur_red = (copy_image[h][w - 1].rgbtRed + copy_image[h][w].rgbtRed + copy_image[h][w + 1].rgbtRed +
                            copy_image[h - 1][w - 1].rgbtRed + copy_image[h - 1][w].rgbtRed + copy_image[h - 1][w + 1].rgbtRed) /
                           6.0;
                blur_blue =
                    (copy_image[h][w - 1].rgbtBlue + copy_image[h][w].rgbtBlue + copy_image[h][w + 1].rgbtBlue +
                     copy_image[h - 1][w - 1].rgbtBlue + copy_image[h - 1][w].rgbtBlue + copy_image[h - 1][w + 1].rgbtBlue) /
                    6.0;
                blur_green =
                    (copy_image[h][w - 1].rgbtGreen + copy_image[h][w].rgbtGreen + copy_image[h][w + 1].rgbtGreen +
                     copy_image[h - 1][w - 1].rgbtGreen + copy_image[h - 1][w].rgbtGreen + copy_image[h - 1][w + 1].rgbtGreen) /
                    6.0;
            }
            else if (h < height && w == 0)
            {
                blur_red = (copy_image[h - 1][w].rgbtRed + copy_image[h - 1][w + 1].rgbtRed + copy_image[h][w].rgbtRed +
                            copy_image[h][w + 1].rgbtRed + copy_image[h + 1][w].rgbtRed + copy_image[h + 1][w + 1].rgbtRed) /
                           6.0;
                blur_blue = (copy_image[h - 1][w].rgbtBlue + copy_image[h - 1][w + 1].rgbtBlue + copy_image[h][w].rgbtBlue +
                             copy_image[h][w + 1].rgbtBlue + copy_image[h + 1][w].rgbtBlue + copy_image[h + 1][w + 1].rgbtBlue) /
                            6.0;
                blur_green =
                    (copy_image[h - 1][w].rgbtGreen + copy_image[h - 1][w + 1].rgbtGreen + copy_image[h][w].rgbtGreen +
                     copy_image[h][w + 1].rgbtGreen + copy_image[h + 1][w].rgbtGreen + copy_image[h + 1][w + 1].rgbtGreen) /
                    6.0;
            }

            else if (h < height && w == width - 1)
            {
                blur_red = (copy_image[h - 1][w - 1].rgbtRed + copy_image[h - 1][w].rgbtRed + copy_image[h][w - 1].rgbtRed +
                            copy_image[h][w].rgbtRed + copy_image[h + 1][w - 1].rgbtRed + copy_image[h + 1][w].rgbtRed) /
                           6.0;
                blur_blue = (copy_image[h - 1][w - 1].rgbtBlue + copy_image[h - 1][w].rgbtBlue + copy_image[h][w - 1].rgbtBlue +
                             copy_image[h][w].rgbtBlue + copy_image[h + 1][w - 1].rgbtBlue + copy_image[h + 1][w].rgbtBlue) /
                            6.0;
                blur_green = (copy_image[h - 1][w - 1].rgbtGreen + copy_image[h - 1][w].rgbtGreen + copy_image[h][w - 1].rgbtGreen +
                              copy_image[h][w].rgbtGreen + copy_image[h + 1][w - 1].rgbtGreen + copy_image[h + 1][w].rgbtGreen) /
                             6.0;
            }

            // Set blur for else pixels

            else
            {
                blur_red = (copy_image[h - 1][w - 1].rgbtRed + copy_image[h - 1][w].rgbtRed + copy_image[h - 1][w + 1].rgbtRed +
                            copy_image[h][w - 1].rgbtRed + copy_image[h][w].rgbtRed + copy_image[h][w + 1].rgbtRed +
                            copy_image[h + 1][w - 1].rgbtRed + copy_image[h + 1][w].rgbtRed + copy_image[h + 1][w + 1].rgbtRed) /
                           9.0;
                blur_blue =
                    (copy_image[h - 1][w - 1].rgbtBlue + copy_image[h - 1][w].rgbtBlue + copy_image[h - 1][w + 1].rgbtBlue +
                     copy_image[h][w - 1].rgbtBlue + copy_image[h][w].rgbtBlue + copy_image[h][w + 1].rgbtBlue +
                     copy_image[h + 1][w - 1].rgbtBlue + copy_image[h + 1][w].rgbtBlue + copy_image[h + 1][w + 1].rgbtBlue) /
                    9.0;
                blur_green =
                    (copy_image[h - 1][w - 1].rgbtGreen + copy_image[h - 1][w].rgbtGreen + copy_image[h - 1][w + 1].rgbtGreen +
                     copy_image[h][w - 1].rgbtGreen + copy_image[h][w].rgbtGreen + copy_image[h][w + 1].rgbtGreen +
                     copy_image[h + 1][w - 1].rgbtGreen + copy_image[h + 1][w].rgbtGreen + copy_image[h + 1][w + 1].rgbtGreen) /
                    9.0;
            }

            image[h][w].rgbtRed = round(blur_red);
            image[h][w].rgbtBlue = round(blur_blue);
            image[h][w].rgbtGreen = round(blur_green);
        }
    }
    return;
}
