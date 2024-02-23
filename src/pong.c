#include "pong.h"
#include "math.h"
#include <SDL.h>

void initialize_game_state(GameState *gameState) {
  gameState->player1.x = PADDLE_SPACE;
  gameState->player1.y = (WINDOW_HEIGHT / 2) - (PADDLE_HEIGHT / 2);
  gameState->player1.height = PADDLE_HEIGHT;
  gameState->player1.width = PADDLE_WIDTH;
  gameState->player1.yVelocity = 0;

  gameState->player2.x = WINDOW_WIDTH - (PADDLE_SPACE);
  gameState->player2.y = (WINDOW_HEIGHT / 2) - (PADDLE_HEIGHT / 2);
  gameState->player2.height = PADDLE_HEIGHT;
  gameState->player2.width = PADDLE_WIDTH;
  gameState->player2.yVelocity = 0;

  gameState->ball.x = WINDOW_WIDTH / 2;
  gameState->ball.y = WINDOW_HEIGHT / 2;
  gameState->ball.radius = 5;
  gameState->ball.speed = INITIAL_BALL_SPEED;
  gameState->ball.xVelocity = gameState->ball.speed * cos(0);
  gameState->ball.yVelocity = gameState->ball.speed * sin(0);

  gameState->scorePlayer1 = 0;
  gameState->scorePlayer2 = 0;

  gameState->paused = SDL_FALSE;
}

void handle_input(SDL_Event e, GameState *gameState) {
  if (e.type == SDL_KEYDOWN) {
    if (!e.key.repeat) {
      switch (e.key.keysym.sym) {
      case SDLK_SPACE:
        gameState->paused = !gameState->paused;
        break;
      case SDLK_r:
        initialize_game_state(gameState);
        break;
      }
    }
    switch (e.key.keysym.sym) {
    case SDLK_UP:
      gameState->player2.yVelocity = -1 * PLAYER_VELOCITY;
      break;
    case SDLK_DOWN:
      gameState->player2.yVelocity = PLAYER_VELOCITY;
      break;
    case SDLK_w:
      gameState->player1.yVelocity = -1 * PLAYER_VELOCITY;
      break;
    case SDLK_s:
      gameState->player1.yVelocity = PLAYER_VELOCITY;
      break;
    }
  }

  if (e.type == SDL_KEYUP) {
    switch (e.key.keysym.sym) {
    case SDLK_UP:
    case SDLK_DOWN:
      gameState->player2.yVelocity = 0;
      break;
    case SDLK_w:
    case SDLK_s:
      gameState->player1.yVelocity = 0;
      break;
    }
  }
}

void update_game_state(GameState *gameState) {
  Paddle *p1 = &gameState->player1;
  Paddle *p2 = &gameState->player2;
  Ball *ball = &gameState->ball;

  p1->y += p1->yVelocity;
  p2->y += p2->yVelocity;

  ball->x += ball->xVelocity;
  ball->y += ball->yVelocity;

  // Collision detection

  Rectangle ballRect;
  ballRect.x = ball->x - ball->radius;
  ballRect.y = ball->y - ball->radius;
  ballRect.height = ball->radius * 2;
  ballRect.width = ball->radius * 2;

  Rectangle leftPaddle;
  leftPaddle.x = p1->x;
  leftPaddle.y = p1->y;
  leftPaddle.height = p1->height;
  leftPaddle.width = p1->width;

  Rectangle rightPaddle;
  rightPaddle.x = p2->x;
  rightPaddle.y = p2->y;
  rightPaddle.height = p2->height;
  rightPaddle.width = p2->width;

  if (checkCollision(&leftPaddle, &ballRect)) {
    float yIntersection = p1->y + PADDLE_HEIGHT / 2 - ball->y;
    float normalizedRelativeIntersectionY = yIntersection / (PADDLE_HEIGHT / 2);
    float bounceAngle = normalizedRelativeIntersectionY * MAX_BOUNCE_ANGLE;
    ball->x = p1->x + p1->width + ball->radius;
    ball->speed += BALL_SPEED_MULTIPLIER;
    ball->xVelocity = ball->speed * cos(bounceAngle);
    ball->yVelocity = ball->speed * sin(bounceAngle) * -1;
  }
  if (checkCollision(&ballRect, &rightPaddle)) {
    float yIntersection = p2->y + PADDLE_HEIGHT / 2 - ball->y;
    float normalizedRelativeIntersectionY = yIntersection / (PADDLE_HEIGHT / 2);
    float bounceAngle = normalizedRelativeIntersectionY * MAX_BOUNCE_ANGLE;

    ball->x = p2->x - ball->radius;
    ball->speed += BALL_SPEED_MULTIPLIER;
    ball->xVelocity = ball->speed * cos(bounceAngle) * -1;
    ball->yVelocity = ball->speed * sin(bounceAngle) * -1;
  }

  if (ball->y - ball->radius < 0 || ball->y + ball->radius > WINDOW_HEIGHT) {
    ball->yVelocity *= -1;
  }

  if (p1->y <= 20) {
    p1->yVelocity = 0;
    p1->y = 20;
  }
  if (p1->y + p1->height > WINDOW_HEIGHT - 20) {
    p1->yVelocity = 0;
    p1->y = (WINDOW_HEIGHT - 20) - p1->height;
  }
  if (p2->y <= 20) {
    p2->y = 20;
    p2->yVelocity = 0;
  }
  if (p2->y + p2->height > WINDOW_HEIGHT - 20) {
    p2->yVelocity = 0;
    p2->y = (WINDOW_HEIGHT - 20) - p2->height;
  }

  if (ball->x < 0) {
    gameState->scorePlayer2 += 1;

    p1->y = WINDOW_HEIGHT / 2 - p1->height / 2;
    p2->y = WINDOW_HEIGHT / 2 - p2->height / 2;

    ball->x = WINDOW_WIDTH / 2;
    ball->y = WINDOW_HEIGHT / 2;
    ball->speed = INITIAL_BALL_SPEED;
    ball->xVelocity = ball->speed * cos(0);
    ball->yVelocity = ball->speed * sin(0);
  }

  if (ball->x > WINDOW_WIDTH) {
    gameState->scorePlayer1 += 1;

    p1->y = WINDOW_HEIGHT / 2 - p1->height / 2;
    p2->y = WINDOW_HEIGHT / 2 - p2->height / 2;

    ball->x = WINDOW_WIDTH / 2;
    ball->y = WINDOW_HEIGHT / 2;
    ball->speed = INITIAL_BALL_SPEED;
    ball->xVelocity = ball->speed * cos(0);
    ball->yVelocity = ball->speed * sin(0);
  }
}

void render_game(SDL_Renderer *renderer, const GameState *gameState) {
  // Clear screen
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  // Draw player1 paddle
  SDL_Rect paddle1 = {gameState->player1.x, gameState->player1.y,
                      gameState->player1.width, gameState->player1.height};
  SDL_RenderFillRect(renderer, &paddle1);

  // Draw player2 paddle
  SDL_Rect paddle2 = {gameState->player2.x - gameState->player2.width,
                      gameState->player2.y, gameState->player2.width,
                      gameState->player2.height};
  SDL_RenderFillRect(renderer, &paddle2);

  // Draw ball
  SDL_Rect ball = {gameState->ball.x - gameState->ball.radius,
                   gameState->ball.y - gameState->ball.radius,
                   gameState->ball.radius * 2, gameState->ball.radius * 2};
  SDL_RenderFillRect(renderer, &ball);

  render_number(renderer, gameState->scorePlayer1,
                gameState->scorePlayer1 >= 10
                    ? (WINDOW_WIDTH / 2) - (2 * (DIGIT_PADDING + DIGIT_WIDTH)) -
                          DIGIT_PADDING
                    : (WINDOW_WIDTH / 2) - (DIGIT_WIDTH + DIGIT_PADDING) -
                          DIGIT_PADDING,
                WINDOW_HEIGHT - (DIGIT_HEIGHT + 50));

  render_number(renderer, gameState->scorePlayer2,
                (WINDOW_WIDTH / 2) + (2 * DIGIT_PADDING),
                WINDOW_HEIGHT - (DIGIT_HEIGHT + 50));

  SDL_RenderPresent(renderer);
}

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

SDL_bool checkCollision(Rectangle *a, Rectangle *b) {
  if (a->x + a->width < b->x)
    return SDL_FALSE;
  if (a->x > b->x + b->width)
    return SDL_FALSE;
  if (a->y + a->height < b->y)
    return SDL_FALSE;
  if (a->y > b->y + b->height)
    return SDL_FALSE;

  return SDL_TRUE;
}