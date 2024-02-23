#include "pong.h"

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window =
      SDL_CreateWindow("Pong",                  // window title
                       50,                      // initial x position
                       SDL_WINDOWPOS_UNDEFINED, // initial y position
                       WINDOW_WIDTH,            // width, in pixels
                       WINDOW_HEIGHT,           // height, in pixels
                       SDL_WINDOW_OPENGL        // flags - see below
      );

  if (window == NULL) {
    printf("Could not create window: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    SDL_DestroyWindow(window);
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  GameState gameState;
  initialize_game_state(&gameState);

  SDL_bool done = SDL_FALSE;
  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        done = SDL_TRUE;
      }
      handle_input(event, &gameState);
    }

    if (!gameState.paused) {
      update_game_state(&gameState);
      render_game(renderer, &gameState);
    }

    SDL_Delay(16); // Approximate 60 FPS
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
