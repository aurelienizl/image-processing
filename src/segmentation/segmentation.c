#include "../tools/tools.h"

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

            if (array[i] >= max)
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

void remove_background(SDL_Surface *image_surface, Uint32 pixel, Uint32 pixel2)
{
    Uint32 white = SDL_MapRGB(image_surface->format, 255, 255, 255);

    int i = 0;
    int j = 0;
    int checked = 1;

    while (i < image_surface->w && checked)
    {
        j = 0;
        while (j < image_surface->h && checked)
        {
            if (get_pixel(image_surface, i, j) == pixel)
            {
                checked = 0;
                break;
            }
            put_pixel(image_surface, i, j, white);
            j++;
        }
        i++;
    }

    i = image_surface->w - 1;
    j = 0;
    checked = 1;

    while (i >= 0 && checked)
    {
        j = 0;
        while (j < image_surface->h && checked)
        {
            if (get_pixel(image_surface, i, j) == pixel)
            {
                checked = 0;
                break;
            }
            put_pixel(image_surface, i, j, white);
            j++;
        }
        i--;
    }

    i = 0;
    j = 0;
    checked = 1;

    while (i < image_surface->h && checked)
    {
        j = 0;
        while (j < image_surface->w && checked)
        {
            if (get_pixel(image_surface, j, i) == pixel2)
            {
                checked = 0;
                break;
            }
            put_pixel(image_surface, j, i, white);
            j++;
        }
        i++;
    }

    i = image_surface->h - 1;
    j = 0;
    checked = 1;

    while (i >= 0 && checked)
    {
        j = 0;
        while (j < image_surface->w && checked)
        {
            if (get_pixel(image_surface, j, i) == pixel2)
            {
                checked = 0;
                break;
            }
            put_pixel(image_surface, j, i, white);
            j++;
        }
        i--;
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
    remove_background(image_surface, red, green);
}