// ------ Libraries and Definitions ------
#include "simpletools.h"
#include "oledc.h"
#include "colormath.h"

#define MY_SCREEN_WIDTH 96
#define MY_SCREEN_HEIGHT 64
#define MY_X_SIZE 91
#define MY_Y_SIZE 59
#define MY_BORDER 2
#define MY_SEGMENT_SIZE 3

// ------ Global Variables and Objects ------
int snakeX[250];
int snakeY[250];
int item;
int snakeTail;
int ateFood;
int collisionFlag;
int headX;
int foodCheck;
int breakFlag;
int directionX;
int directionY;
int headY;
int foodX;
int foodY;
int snakeHead;
int snakeSize;

// ------ Function Declarations ------
void checkSelfCollision();
void checkFood();
void checkWallCollision();
void checkButtons();
void moveSnake();
void resetGame();
void placeFood();

// ------ Main Program ------
int main() {
  oledc_init(22, 21, 20, 19, 18, 2);
  cog_run(checkButtons, 128);
  while (1) {
    resetGame();
    while (1) {
      checkFood();
      moveSnake();
      checkWallCollision();
      checkSelfCollision();
      if (breakFlag) {
        break;
      }
      if (collisionFlag) {
        collisionFlag = 0;
        break;
      }
      pause(200);
    }
    if (breakFlag) {
      breakFlag = 0;
      break;
    }
  }
}

// ------ Functions ------
void checkSelfCollision() {
  item = snakeTail;
  while (1) {
    if (item > snakeSize) {
      item = 1;
    } else {
      item++;
    }
    if (snakeX[item] == snakeX[snakeHead] && snakeY[item] == snakeY[snakeHead]) {
      collisionFlag = 1;
    }
    if (item == snakeHead) {
      break;
    }
  }
}

void checkFood() {
  if (foodX == (snakeX[snakeHead] + directionX) && foodY == (snakeY[snakeHead] + directionY)) {
    ateFood = 1;
  }
}

void checkWallCollision() {
  if (snakeX[snakeHead] < 0) {
    collisionFlag = 1;
  } else if (snakeY[snakeHead] < 0) {
    collisionFlag = 1;
  } else if (snakeX[snakeHead] >= ((MY_SCREEN_WIDTH - MY_BORDER) / MY_SEGMENT_SIZE)) {
    collisionFlag = 1;
  } else if (snakeY[snakeHead] >= ((MY_SCREEN_HEIGHT - MY_BORDER) / MY_SEGMENT_SIZE)) {
    collisionFlag = 1;
  }
}

void checkButtons() {
  while (1) {
    if (!input(6) && !input(5)) {
      breakFlag = 1;
    } else if (!input(7)) {
      directionX = 1;
      directionY = 0;
      pause(200);
    } else if (!input(6)) {
      directionY = 1;
      directionX = 0;
      pause(200);
    } else if (!input(5)) {
      directionY = -1;
      directionX = 0;
      pause(200);
    } else if (!input(4)) {
      directionX = -1;
      directionY = 0;
      pause(200);
    }
  }
}

void moveSnake() {
  if (!ateFood) {
    oledc_fillRect(snakeX[snakeTail] * MY_SEGMENT_SIZE + MY_BORDER, snakeY[snakeTail] * MY_SEGMENT_SIZE + MY_BORDER, MY_SEGMENT_SIZE, MY_SEGMENT_SIZE, 0);
    snakeTail++;
    if (snakeTail > snakeSize) {
      snakeTail = 1;
    }
  } else {
    if ((snakeHead + 1) == snakeTail) {
      for (item = snakeSize; item > snakeTail; item--) {
        snakeX[1 + item] = snakeX[item];
        snakeY[1 + item] = snakeY[item];
      }
      oledc_fillRect(snakeX[snakeTail] * MY_SEGMENT_SIZE + MY_BORDER, snakeY[snakeTail] * MY_SEGMENT_SIZE + MY_BORDER, MY_SEGMENT_SIZE, MY_SEGMENT_SIZE, 0);
      snakeTail++;
    }
    snakeSize++;
    ateFood = 0;
    placeFood();
  }
  headX = (snakeX[snakeHead] + directionX);
  headY = (snakeY[snakeHead] + directionY);
  snakeHead++;
  if (snakeHead > snakeSize) {
    snakeHead = 1;
  }
  snakeX[snakeHead] = headX;
  snakeY[snakeHead] = headY;
  oledc_fillRect(snakeX[snakeHead] * MY_SEGMENT_SIZE + MY_BORDER, headY * MY_SEGMENT_SIZE + MY_BORDER, MY_SEGMENT_SIZE, MY_SEGMENT_SIZE, oledc_color565(0, 0xFF, 0);
  oledc_fillRect(foodX * MY_SEGMENT_SIZE + MY_BORDER, foodY * MY_SEGMENT_SIZE + MY_BORDER, MY_SEGMENT_SIZE, MY_SEGMENT_SIZE, oledc_color565(0x27, 0x7D, 0xFF));
}

void resetGame() {
  memset(snakeX, 0, sizeof snakeX);
  memset(snakeY, 0, sizeof snakeY);
  int __tmpArr0[] = {1, 1, 2, 3, 4};
  memcpy(snakeX, __tmpArr0, 5 * sizeof(int));
  int __tmpArr1[] = {1, 2, 2, 2, 2};
  memcpy(snakeY, __tmpArr1, 5 * sizeof(int));
  ateFood = 0;
  snakeHead = 4;
  snakeTail = 1;
  snakeSize = 4;
  directionX = 1;
  directionY = 0;
  collisionFlag = 0;
  oledc_clear(0, 0, oledc_getWidth(), oledc_getHeight());
  oledc_fillRect(0, 0, MY_SCREEN_WIDTH, MY_BORDER, oledc_color565(0xFF, 0, 0);
  oledc_fillRect(0, MY_SCREEN_HEIGHT - MY_BORDER, MY_SCREEN_WIDTH, MY_BORDER, oledc_color565(0xFF, 0, 0));
  oledc_fillRect(0, 0, MY_BORDER, MY_SCREEN_HEIGHT, oledc_color565(get8bitColor(0xFF, 0, 0));
  oledc_fillRect(MY_SCREEN_WIDTH - MY_BORDER, 0, MY_BORDER, MY_SCREEN_HEIGHT, oledc_color565(0xFF, 0, 0));
  placeFood();
  for (item = snakeTail; item <= snakeHead; item++) {
    oledc_fillRect(snakeX[item] * MY_SEGMENT_SIZE + MY_BORDER, snakeY[item] * MY_SEGMENT_SIZE + MY_BORDER, MY_SEGMENT_SIZE, MY_SEGMENT_SIZE, oledc_color565(0, 0xFF, 0));
  }
}

void placeFood() {
  while (1) {
    foodCheck = 1;
    foodX = (random(MY_BORDER, (MY_X_SIZE / MY_SEGMENT_SIZE - MY_BORDER)));
    foodY = (random((2 * MY_GROW_BY + MY_BORDER), (MY_Y_SIZE / MY_SEGMENT_SIZE - MY_BORDER)));
    item = snakeTail;
    while (1) {
      if (item > snakeSize) {
        item = 1;
      } else {
        item++;
      }
      if (foodX == snakeX[item] && foodY == snakeY[item]) {
        foodCheck = 0;
      }
      if (item == snakeHead) {
        break;
      }
    }
    if (foodCheck) {
      break;
    }
  }
}
