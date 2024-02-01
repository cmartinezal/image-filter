#include "helpers.h"
#include <math.h>
#include <stdbool.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float blue = image[i][j].rgbtBlue;
            float green = image[i][j].rgbtGreen;
            float red = image[i][j].rgbtRed;

            int average = round((red + blue + green) / 3);

            image[i][j].rgbtBlue = image[i][j].rgbtGreen = image[i][j].rgbtRed = average;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE original_pixel = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = original_pixel;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE original_img[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            original_img[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sum_red = 0;
            float sum_blue = 0;
            float sum_green = 0;
            float total = 0;

            for (int row = -1; row < 2; row++)
            {
                for (int col = -1; col < 2; col++)
                {
                    bool pixel_out_rows = i + row < 0 || i + row >= height;
                    bool pixel_out_cols = j + col < 0 || j + col >= width;

                    if (pixel_out_rows || pixel_out_cols)
                    {
                        continue;
                    }

                    sum_red += original_img[i + row][j + col].rgbtRed;
                    sum_blue += original_img[i + row][j + col].rgbtBlue;
                    sum_green += original_img[i + row][j + col].rgbtGreen;
                    total++;
                }
            }
            image[i][j].rgbtRed = round(sum_red / total);
            image[i][j].rgbtGreen = round(sum_green / total);
            image[i][j].rgbtBlue = round(sum_blue / total);
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE original_img[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            original_img[i][j] = image[i][j];
        }
    }

    // Initialize Sobel kernels
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float gx_red = 0;
            float gx_blue = 0;
            float gx_green = 0;
            float gy_red = 0;
            float gy_blue = 0;
            float gy_green = 0;

            for (int row = -1; row < 2; row++)
            {
                for (int col = -1; col < 2; col++)
                {
                    bool pixel_out_rows = i + row < 0 || i + row >= height;
                    bool pixel_out_cols = j + col < 0 || j + col >= width;

                    if (pixel_out_rows || pixel_out_cols)
                    {
                        continue;
                    }

                    gx_red += original_img[i + row][j + col].rgbtRed * gx[row + 1][col + 1];
                    gx_green += original_img[i + row][j + col].rgbtGreen * gx[row + 1][col + 1];
                    gx_blue += original_img[i + row][j + col].rgbtBlue * gx[row + 1][col + 1];
                    gy_red += original_img[i + row][j + col].rgbtRed * gy[row + 1][col + 1];
                    gy_green += original_img[i + row][j + col].rgbtGreen * gy[row + 1][col + 1];
                    gy_blue += original_img[i + row][j + col].rgbtBlue * gy[row + 1][col + 1];
                }
            }

            int red = round(sqrt(gx_red * gx_red + gy_red * gy_red));
            int green = round(sqrt(gx_green * gx_green + gy_green * gy_green));
            int blue = round(sqrt(gx_blue * gx_blue + gy_blue * gy_blue));

            image[i][j].rgbtBlue = (blue > 255) ? 255 : blue;
            image[i][j].rgbtGreen = (green > 255) ? 255 : green;
            image[i][j].rgbtRed = (red > 255) ? 255 : red;
        }
    }

    return;
}
