#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// BYTE  rgbtBlue;
// BYTE  rgbtGreen;
// BYTE  rgbtRed;

// Convert image to grayscale

int cap(int rgb);
RGBTRIPLE neighbors_average(int i, int j, int height, int width, RGBTRIPLE image[height][width]);
RGBTRIPLE calc_kernels(int i, int j, int height, int width, RGBTRIPLE **image);

void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int average = round((image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen) / 3.0);
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
    for (int i = 0; i < height; i++)
    {
        int start = 0;
        int end = width - 1;
        RGBTRIPLE temp;
        while (start < end)
        {
            temp = image[i][start];
            image[i][start] = image[i][end];
            image[i][end] = temp;
            start++;
            end--;
        }
    }
    return;
}

// return RGBTRIPLE average of neighbors elements of a 2d array
RGBTRIPLE neighbors_average(int i, int j, int height, int width, RGBTRIPLE image[height][width])
{
    int row_limit = height - 1;
    int col_limit = width - 1;
    int sum_blue = 0;
    int sum_green = 0;
    int sum_red = 0;
    float count = 0;

    for (int x = fmax(0, i - 1); x <= fmin(i + 1, row_limit); x++)
    {
        for (int y = fmax(0, j - 1); y <= fmin(j + 1, col_limit); y++)
        {
            sum_blue += image[x][y].rgbtBlue;
            sum_green += image[x][y].rgbtGreen;
            sum_red += image[x][y].rgbtRed;
            count += 1.0;
        }
    }

    RGBTRIPLE average_triple;
    average_triple.rgbtBlue = round((float) sum_blue / count);
    average_triple.rgbtGreen = round((float) sum_green / count);
    average_triple.rgbtRed = round((float) sum_red / count);

    return average_triple;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE result[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            result[i][j] = neighbors_average(i, j, height, width, image);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = result[i][j];
        }
    }

    return;
}

int cap(int rgb)
{
    if (rgb > 255)
    {
        return 255;
    }
    else
    {
        return rgb;
    }
}

RGBTRIPLE calc_kernels(int i, int j, int height, int width, RGBTRIPLE **image)
{
    int kernel_x[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int kernel_y[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    int kernel_index = 0;
    int sum_x_blue = 0, sum_x_green = 0, sum_x_red = 0;
    int sum_y_blue = 0, sum_y_green = 0, sum_y_red = 0;

    for (int x = i - 1; x <= i + 1; x++)
    {
        for (int y = j - 1; y <= j + 1; y++)
        {
            sum_x_blue += image[x][y].rgbtBlue * kernel_x[kernel_index];
            sum_x_green += image[x][y].rgbtGreen * kernel_x[kernel_index];
            sum_x_red += image[x][y].rgbtRed * kernel_x[kernel_index];

            sum_y_blue += image[x][y].rgbtBlue * kernel_y[kernel_index];
            sum_y_green += image[x][y].rgbtGreen * kernel_y[kernel_index];
            sum_y_red += image[x][y].rgbtRed * kernel_y[kernel_index];

            kernel_index++;
        }
    }

    RGBTRIPLE result;

    result.rgbtBlue = cap(round(sqrt(sum_x_blue * sum_x_blue + sum_y_blue * sum_y_blue)));
    result.rgbtGreen = cap(round(sqrt(sum_x_green * sum_x_green + sum_y_green * sum_y_green)));
    result.rgbtRed = cap(round(sqrt(sum_x_red * sum_x_red + sum_y_red * sum_y_red)));

    return result;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // printf("pixel 00 red: %i\n", image[0][0].rgbtRed);
    // Create a padded image
    int padded_height = height + 2;
    int padded_width = width + 2;

    RGBTRIPLE **padded_image = malloc(padded_height * sizeof(RGBTRIPLE *));
    for (int i = 0; i < padded_height; i++)
    {
        padded_image[i] = malloc(padded_width * sizeof(RGBTRIPLE));
        for (int j = 0; j < padded_width; j++)
        {
            padded_image[i][j].rgbtBlue = 0;
            padded_image[i][j].rgbtGreen = 0;
            padded_image[i][j].rgbtRed = 0;
        }
    }
    // printf("pixel 00 red: %i\n", padded_image[0][0].rgbtRed);

    // Copy original image to the center of padded_image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            padded_image[i + 1][j + 1] = image[i][j];
        }
    }

    RGBTRIPLE result[height][width];

    for (int i = 1; i < height + 1; i++)
    {
        for (int j = 1; j < width + 1; j++)
        {
            result[i - 1][j - 1] = calc_kernels(i, j, height, width, padded_image);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = result[i][j];
        }
    }

    for (int i = 0; i < padded_height; i++)
    {
        free(padded_image[i]);
    }
    free(padded_image);


    return;
}
