#include "tools.h"

//Apply grayscale
void grayscale(SDL_Surface *image_surface)
{
    Uint8 r, g, b;

    for (int i = 0; i < image_surface->w; i++)
    {
        for (int j = 0; j < image_surface->h; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            r =  0.3 * (double) r + 0.59 * (double) g + 0.11 * (double) b;

            pixel = SDL_MapRGB(image_surface->format, r, r, r);
            put_pixel(image_surface, i, j, pixel);
        }
    }
}

//Invert all colors from surface
void invert(SDL_Surface *image_surface)
{
    Uint8 r, g, b;

    for (int i = 0; i < image_surface->w; i++)
    {
        for (int j = 0; j < image_surface->h; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            r = 255 - r;
            g = 255 - g;
            b = 255 - b;

            pixel = SDL_MapRGB(image_surface->format, r, g, b);
            put_pixel(image_surface, i, j, pixel);
        }
    }
}

//Increases or reduces the contrast 
void contrast_v1(SDL_Surface* image_surface, int delta)
{
    double factor = (259 * (delta + 255)) / (255.0 * (259.0 - delta));
    Uint32 pixel;
    Uint8 r, g, b;

    if (delta == 259){ delta = 258;}
    
    for (int i = 0; i < image_surface -> w;i++)
    {
        for(int j = 0; j< image_surface -> h; j++)
        {
            pixel = get_pixel(image_surface, i,j);
            SDL_GetRGB(pixel, image_surface->format, &r,&g,&b);
            r = StayOnInterval256(factor * (r - 128) + 128);
            g = StayOnInterval256(factor * (g - 128) + 128);
            b = StayOnInterval256(factor * (b - 128) + 128);
            pixel = SDL_MapRGB(image_surface -> format,r,g,b);
            put_pixel(image_surface,i,j,pixel);
        }
    }
}


//Shows borders from objects inside surface (by color difference)
void edges_detection(SDL_Surface *image_surface,int threshold)
{
    Uint8 r_old, g_old, b_old;
    Uint8 r_left, g_left, b_left;
    Uint8 r_bottom, g_bottom, b_bottom;

    for (int y = 0; y < image_surface -> w - 1; y++)
    {
        for (int x = 1; x < image_surface -> h; x++)
        {
            Uint32 old_pixel = get_pixel(image_surface, x, y);
            Uint32 left_pixel = get_pixel(image_surface, x - 1, y);
            Uint32 bottom_pixel =  get_pixel(image_surface, x, y + 1);
            SDL_GetRGB(old_pixel, image_surface->format, &r_old, &g_old, &b_old);
            SDL_GetRGB(left_pixel, image_surface->format, &r_left, &g_left, &b_left);
            SDL_GetRGB(bottom_pixel, image_surface->format, &r_bottom, &g_bottom, &b_bottom);

            int oldValue = (r_old + g_old + b_old)/3;
            int leftValue = (r_left + g_left + b_left)/3;
            int bottomValue = (r_bottom + g_bottom + b_bottom)/3;

            if (abs(oldValue - leftValue) <= threshold || abs(oldValue - bottomValue) <= threshold)
            {
                Uint32 pixel;
                pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
                put_pixel(image_surface, x, y, pixel);
            }

        }
    }
}

//Used by [blur] function, return the average of 9 pixels in an array
Uint32 average_blur(SDL_Surface *image_surface,int array[])
{
    unsigned int average_red = 0;
    unsigned int average_green = 0;
    unsigned int average_blue = 0;

    Uint8 r,g,b;

    for (size_t i = 0; i < 9; i++)
    {
        SDL_GetRGB(array[i],image_surface->format,&r,&g,&b);
        average_red += r;
        average_green += g;
        average_blue += b;
    }
    
    average_red /= 9;
    average_green /= 9;
    average_blue /= 9;

    Uint32 pixel = SDL_MapRGB(image_surface->format, average_red,average_green,average_blue);
    return pixel;
}

//Blur filter, using the average of 3x3 pixels 
void blur(SDL_Surface *image_surface)
{
    int array[9];
    SDL_Surface *blur_surface = image_surface;
    for (int i = 1; i < image_surface->w - 1; i++)
    {
        for (int j = 1; j < image_surface->h - 1; j++)
        {
            int index = 0;
            for (int k = i-1; k <= i + 1; k++)
            {
                for (int l = j - 1; l <= j + 1; l++)
                {
                    array[index] = get_pixel(image_surface,k,l);
                    index++;
                }
            }
            put_pixel(blur_surface,i,j,average_blur(image_surface,array));
        }
    }
    *image_surface = *blur_surface;
}

//Reduce noise (works only for 1x1 pixel and binarized surface)
void noiseReduction(SDL_Surface *image_surface)
{
    int table[5];

    for (int i = 0; i < image_surface -> w; i++)
    {
        for (int j = 0; j < image_surface -> h; j++)
        {

            for (int k = j; k <= j + 4; k++)
            {
                if (i == 0)
                {
                    if (k == 0)
                    {
                        table[0] = get_pixel(image_surface, i, k);
                        table[1] = get_pixel(image_surface, i, k);
                        table[2] = get_pixel(image_surface, i, k);
                        table[3] = get_pixel(image_surface, i, k + 1);
                        table[4] = get_pixel(image_surface, i + 1, k);
                        break;
                    }
                    if (k == image_surface -> h)
                    {
                        table[0] = get_pixel(image_surface, i, k);
                        table[1] = get_pixel(image_surface, i, k - 1);
                        table[2] = get_pixel(image_surface, i, k);
                        table[3] = get_pixel(image_surface, i, k);
                        table[4] = get_pixel(image_surface, i + 1, k);
                        break;
                    }
                    else
                    {
                        table[0] = get_pixel(image_surface, i, k);
                        table[1] = get_pixel(image_surface, i, k - 1);
                        table[2] = get_pixel(image_surface, i, k);
                        table[3] = get_pixel(image_surface, i, k + 1);
                        table[4] = get_pixel(image_surface, i + 1, k);
                        break;
                    }
                }
                if (i == image_surface -> w)
                {
                    if (k == 0)
                    {
                        table[0] = get_pixel(image_surface, i, k);
                        table[1] = get_pixel(image_surface, i, k);
                        table[2] = get_pixel(image_surface, i - 1, k);
                        table[3] = get_pixel(image_surface, i, k + 1);
                        table[4] = get_pixel(image_surface, i, k);
                        break;
                    }
                    if (k == image_surface -> h)
                    {
                        table[0] = get_pixel(image_surface, i, k);
                        table[1] = get_pixel(image_surface, i, k - 1);
                        table[2] = get_pixel(image_surface, i - 1, k);
                        table[3] = get_pixel(image_surface, i, k);
                        table[4] = get_pixel(image_surface, i, k);
                        break;
                    }
                    else
                    {
                        table[0] = get_pixel(image_surface, i, k);
                        table[1] = get_pixel(image_surface, i, k - 1);
                        table[2] = get_pixel(image_surface, i - 1, k);
                        table[3] = get_pixel(image_surface, i, k + 1);
                        table[4] = get_pixel(image_surface, i, k);
                        break;
                    }
                }
                if (k == 0)
                {
                    table[0] = get_pixel(image_surface, i, k);
                    table[1] = get_pixel(image_surface, i, k);
                    table[2] = get_pixel(image_surface, i - 1, k);
                    table[3] = get_pixel(image_surface, i, k + 1);
                    table[4] = get_pixel(image_surface, i + 1, k);
                    break;
                }
                if (k == image_surface -> h)
                {
                    table[0] = get_pixel(image_surface, i, k);
                    table[1] = get_pixel(image_surface, i, k - 1);
                    table[2] = get_pixel(image_surface, i - 1, k);
                    table[3] = get_pixel(image_surface, i, k);
                    table[4] = get_pixel(image_surface, i + 1, k);
                    break;
                }
                else
                {
                    table[0] = get_pixel(image_surface, i, k);
                    table[1] = get_pixel(image_surface, i, k - 1);
                    table[2] = get_pixel(image_surface, i - 1, k);
                    table[3] = get_pixel(image_surface, i, k + 1);
                    table[4] = get_pixel(image_surface, i + 1, k);
                    break;
                }
            }
            array_select_sort(table, 5);
            put_pixel(image_surface, i, j, table[2]);
        }
    }
}

//Create a histogram 
void init_hist(SDL_Surface *image_surface, int width, int height, float *hist)
{
    Uint8 r, g, b;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            hist[r]++;
        }
    }
}

void normalized(float *hist, int Pixels)
{
    for (int i = 0; i < 256; i++)
        hist[i] = hist[i] / Pixels;
}

Uint8 otsu_threshold(float *hist)
{
    float w0 = 0, w1 = 0, wT = 0;
    float u0 = 0, u1 = 0, uT = 0;
    float sum = 0, vk = 0, v_max = 0;
    float threshold = 0;

    for (size_t i = 0; i < 256; i++)
    {
        uT += i * hist[i];
        wT += hist[i];
    }

    for (size_t i = 0; i < 256; i++)
    {
        w0 += hist[i];
        w1 = wT - w0;

        sum += i * hist[i];
        u0 = sum / w0;
        u1 = (uT - sum) / w1;

        // Maximizing inter-class variance
        vk = w0 * w1 * (u0 - u1) * (u0 - u1);

        // Find max vk = Find threshold
        if (vk > v_max)
        {
            threshold = i;
            v_max = vk;
        }
    }

    return (Uint8)threshold;
}

//OTSU filters, for more info see wiki 
void otsu(SDL_Surface *image_surface)
{
    size_t width = image_surface->w;
    size_t height = image_surface->h;

    float hist[256] = {0};
    init_hist(image_surface, width, height, hist);
    normalized(hist, width * height);

    Uint8 threshold = otsu_threshold(hist);
    Uint8 r, g, b;

    for (size_t i = 0; i < width; i++)
    {
        for (size_t j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            if (r > threshold)
                r = 255;
            else
                r = 0;

            pixel = SDL_MapRGB(image_surface->format, r, r, r);
            put_pixel(image_surface, i, j, pixel);
        }
    }
}