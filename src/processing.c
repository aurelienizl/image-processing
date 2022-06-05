#include "filters/filters.h"
#include "segmentation/segmentation.h"
#include "tools/tools.h"

// image processing parameters
int contrast = -10;
int binarise = 40;

void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1, "Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface *load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

void SDL_FreeSurface(SDL_Surface *surface);

void save(SDL_Surface *image_surface)
{
    // Save original image as output.bmp
    SDL_SaveBMP(image_surface, "output.bmp");
}

void applyFilters(SDL_Surface *image_surface)
{
    contrast_v1(image_surface, contrast);
    blur(image_surface, 1);
    binarize(image_surface, binarise);
    noiseReduction_v2(image_surface, 5);
    invert(image_surface);
    save(image_surface);
}

void applySegmentation(SDL_Surface *image_surface)
{
    detect_lines_v2(image_surface);
    save(image_surface);
}

int main(int argc, char *array[])
{
    // Exit if inputs are not correct
    if (argc != 3)
    {
        printf("Usage: ./processing [PATH_TO_FILE] [DISPLAY PROCESS 1 OR 0]\n");
        return EXIT_FAILURE;
    }

    SDL_Surface *image_surface;

    // initialise sdl
    init_sdl();

    // Load image into memory from path
    image_surface = load_image(array[1]);

    if (*array[2] == 49)
    {
        display_image(image_surface);
        wait_for_keypressed();

        applyFilters(image_surface);
        applySegmentation(image_surface);

        display_image(image_surface);
        wait_for_keypressed();
    }
    else
    {
        applyFilters(image_surface);
        applySegmentation(image_surface);
    }

    // Saving image with applied filters
    // clear and exit
    SDL_FreeSurface(image_surface);

    return 0;
}
