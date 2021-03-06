#ifndef FILTERS_TOOLS_H
#define FILTERS_TOOLS_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
void update_surface(SDL_Surface *screen, SDL_Surface *image);
void array_swap(int array[], size_t i, size_t j);
void array_select_sort(int array[], size_t len);

Uint8 *pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);
Uint8 max_pixel(SDL_Surface *image_surface, int width, int height);
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);

int StayOnInterval256(int n);

SDL_Surface *display_image(SDL_Surface *img);
void wait_for_keypressed();

#endif // FILTERS_TOOLS_H
