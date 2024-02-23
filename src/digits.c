#include "digits.h"

// Each digit's representation is a 3x5 matrix, where 1 indicates to draw a
// rectangle
int digits[10][15] = {
    {1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1}, // 0
    {0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0}, // 1
    {1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1}, // 2
    {1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1}, // 3
    {1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1}, // 4
    {1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1}, // 5
    {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, // 7
    {1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1}  // 9
};

void render_digit(SDL_Renderer *renderer, int digit, int x, int y) {
  if (digit < 0 || digit > 9)
    return; // Check for valid digit

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color

  for (int row = 0; row < 5; row++) {
    for (int col = 0; col < 3; col++) {
      if (digits[digit][row * 3 + col] == 1) {
        SDL_Rect rect = {x + col * (DIGIT_WIDTH / 3),
                         y + row * (DIGIT_HEIGHT / 5), DIGIT_WIDTH / 3,
                         DIGIT_HEIGHT / 5};
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }
}

void render_number(SDL_Renderer *renderer, int number, int x, int y) {

  if (number == 0) {
    render_digit(renderer, 0, x, y);
    return;
  }
  int numDigits = 0;
  int tempNumber = number;

  // Count the number of digits in the number
  do {
    numDigits++;
    tempNumber /= 10;
  } while (tempNumber != 0);

  tempNumber = number;
  int digitPosition = numDigits - 1;

  // Render each digit
  while (tempNumber != 0) {
    int digit = tempNumber % 10;
    render_digit(renderer, digit,
                 x + digitPosition * (DIGIT_WIDTH + DIGIT_PADDING), y);
    tempNumber /= 10;
    digitPosition--;
  }
}
