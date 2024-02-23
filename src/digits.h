#ifndef DIGITS_H
#define DIGITS_H

#include <SDL.h>

#define DIGIT_WIDTH 20
#define DIGIT_HEIGHT 30
#define DIGIT_PADDING 5

void render_digit(SDL_Renderer *renderer, int digit, int x, int y);
void render_number(SDL_Renderer *renderer, int number, int x, int y);


#endif 