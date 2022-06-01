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
void detect_lines(SDL_Surface *image_surface)
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