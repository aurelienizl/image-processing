#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <stdlib.h>
#include <SDL/SDL.h>
#include <err.h>
#include <SDL/SDL_image.h>

void detect_lines(SDL_Surface *image_surface);

#endif