#include "../tools/tools.h"

int __detect_lines(SDL_Surface *image_surface, int *array, int len)
{
    Uint8 r, g, b;
    int count = 0;
    int tolerance = len;
    for (int i = 0; i < len; i++)
    {
        SDL_GetRGB(array[i], image_surface->format, &r, &g, &b);
        if ((r) == 0)
        {
            count += 1;
        }
    }
    if (count > tolerance / 3)
    {
        return 1;
    }
    return 0;
}

// works only with binarized pictures (black and white)
void detect_lines_v1(SDL_Surface *image_surface)
{
    int array_w[image_surface->w];
    int val = 1;
    int rows = 0;
    int scale = 50;
    // int array_h[image_surface->h];

    for (int i = 0; i < image_surface->w; i++)
    {
        if (i % scale == 0)
        {
            val = 1;
        }
        for (int j = 0; j < image_surface->h; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            array_w[j] = pixel;
        }
        if (__detect_lines(image_surface, array_w, image_surface->w))
        {
            if (val && rows < 10)
            {
                for (int k = 0; k < image_surface->h; k++)
                {
                    Uint32 pixelb = SDL_MapRGB(image_surface->format, 255, 0, 0);
                    put_pixel(image_surface, i, k, pixelb);
                }
                val = 0;
                rows++;
            }
        }
    }
}

void detect_lines_array(int *array, int len)
{
    int lines = 10;
    int lines_scale = 30;
    for (int i = 0; i < lines; i++)
    {
        int max = array[0];
        int index = 0;
        for (int i = 0; i < len; i++)
        {

            if (array[i] > max)
            {
                max = array[i];
                index = i;
            }
        }
        array[index] = -1;
        for (int i = 1; i < lines_scale; i++)
        {
            if (index + i < len)
            {
                array[index + i] = 0;
            }
            if (index - i >= 0)
            {
                array[index - i] = 0;
            }
        }
    }
}

void remove_background(SDL_Surface *image_surface, Uint32 pixel)
{
    Uint32 white = SDL_MapRGB(image_surface->format, 255, 255, 255);
    for (int i = 0; i < image_surface->w; i++)
    {
        for (int j = 0; j < image_surface->h; j++)
        {
            if (get_pixel(image_surface, i, j) == pixel)
            {
                return;
            }
            put_pixel(image_surface, i, j, white);
        }
    }
}

// works only with binarized pictures (black and white)
void detect_lines_v2(SDL_Surface *image_surface)
{
    Uint8 r, g, b;
    // Uint32 red = SDL_MapRGB(image_surface->format, 255, 0, 0);
    int *array_w = calloc(image_surface->w, sizeof(int));
    int *array_h = calloc(image_surface->h, sizeof(int));

    for (int i = 0; i < image_surface->w; i++)
    {
        for (int j = 0; j < image_surface->h; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            if (r == 0)
            {
                array_w[i] += 1;
            }
        }
    }

    for (int i = 0; i < image_surface->h; i++)
    {
        for (int j = 0; j < image_surface->w; j++)
        {
            Uint32 pixel = get_pixel(image_surface, j, i);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            if (r == 0)
            {
                array_h[i] += 1;
            }
        }
    }

    Uint32 red = SDL_MapRGB(image_surface->format, 255, 0, 0);
    Uint32 green = SDL_MapRGB(image_surface->format, 0, 255, 0);

    detect_lines_array(array_h, image_surface->h);
    detect_lines_array(array_w, image_surface->w);

    for (int i = 0; i < image_surface->w; i++)
    {
        for (int j = 0; j < image_surface->h; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            if (array_w[i] == -1)
            {
                put_pixel(image_surface, i, j, red);
            }
        }
    }

    for (int i = 0; i < image_surface->h; i++)
    {
        for (int j = 0; j < image_surface->w; j++)
        {
            Uint32 pixel = get_pixel(image_surface, j, i);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            if (array_h[i] == -1)
            {
                put_pixel(image_surface, j, i, green);
            }
        }
    }
    remove_background(image_surface, red);
    remove_background(image_surface, green);
}