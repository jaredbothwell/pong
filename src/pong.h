#ifndef PONG_H
#define PONG_H

#include <SDL.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define PADDLE_WIDTH 10
#define PADDLE_HEIGHT 80
#define PADDLE_SPACE 50
#define PLAYER_VELOCITY 20

#define DIGIT_WIDTH 20
#define DIGIT_HEIGHT 30
#define DIGIT_PADDING 5

#define PI 3.14159
#define INITIAL_BALL_SPEED 10
#define BALL_SPEED_MULTIPLIER .25
#define MAX_BOUNCE_ANGLE PI / 4

typedef struct {
  float x, y;
  float width, height;
  float yVelocity;
} Paddle;

typedef struct {
  float x, y;
  float xVelocity, yVelocity, speed;
  float radius;
} Ball;

typedef struct {
  Paddle player1, player2;
  Ball ball;
  int scorePlayer1, scorePlayer2;
  SDL_bool paused;
} GameState;

typedef struct {
  float x, y; // Top-left corner
  float width, height;
} Rectangle;

void initialize_game_state(GameState *gameState);
void handle_input(SDL_Event e, GameState *gameState);
void update_game_state(GameState *gameState);
void render_game(SDL_Renderer *renderer, const GameState *gameState);
void render_digit(SDL_Renderer *renderer, int digit, int x, int y);
void render_number(SDL_Renderer *renderer, int number, int x, int y);
SDL_bool checkCollision(Rectangle *a, Rectangle *b);

#endif