#ifndef Filters_H_
#define FILTERS_H

#include <stdlib.h>
#include <SDL/SDL.h>
#include <err.h>
#include <SDL/SDL_image.h>


//Filters functions:

void grayscale(SDL_Surface* screen);
void contrast_v1(SDL_Surface* img, int delta);
void invert(SDL_Surface *image_surface);
void edges_detection(SDL_Surface *image_surface, int threshold);
void noiseReduction(SDL_Surface *surface);
void blur(SDL_Surface *image_surface);
void otsu(SDL_Surface *image_surface);

#endif
