#include "filters/filters.h"
#include "segmentation/segmentation.h"

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

SDL_Surface *display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE | SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
             img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if (SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}

void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while (event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while (event.type != SDL_KEYUP);
}

void SDL_FreeSurface(SDL_Surface *surface);

void save(SDL_Surface *image_surface)
{
    // Save original image as output.bmp
    SDL_SaveBMP(image_surface, "output.bmp");
}

void applyFilters(SDL_Surface *image_surface)
{
    contrast_v1(image_surface, 100);
    binarize(image_surface, 10);
    invert(image_surface);
    save(image_surface);
}

void applySegmentation(SDL_Surface *image_surface)
{
    detect_lines_v1(image_surface);
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
