// ------ Libraries and Definitions ------
#include "simpletools.h"
#include "oledc.h"
#include "colormath.h"

// ------ Global Variables and Objects ------
int bricks[45];
int paddleX;
int paddleWidth;
int brickCount;
int ballSpeedX;
int ballSpeedY;
int ballX;
int ballY;
int brickY;
int brickX;
int paddleCenter;



// ------ Main Program ------
int main() {
  oledc_init(22, 21, 20, 19, 18, 2);
  while (1) {
    int __tmpArr6[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    memcpy(bricks, __tmpArr6, 45 * sizeof(int));
    paddleX = 45;
    paddleWidth = 10;
    brickCount = 45;
    ballSpeedX = 3;
    ballSpeedY = -6;
    ballX = 200;
    ballY = 500;
    oledc_fillRect(0, 0, oledc_getWidth(), oledc_getHeight(), oledc_color565(get8bitColor(0x33cc00, "RED"), get8bitColor(0x33cc00, "GREEN"), get8bitColor(0x33cc00, "BLUE")));
    oledc_fillRect(2, 2, (oledc_getWidth()) - 4, oledc_getHeight(), oledc_color565(get8bitColor(0x000000, "RED"), get8bitColor(0x000000, "GREEN"), get8bitColor(0x000000, "BLUE")));
    for (brickY = 0; brickY < 3; brickY++) {
      for (brickX = 0; brickX < 15; brickX++) {
        oledc_fillRect(brickX * 6 + 3, brickY * 5 + 3, 5, 4, oledc_color565(get8bitColor(getColorRRGGBB(brickX * 17, 0, 255 - brickX * 17), "RED"), get8bitColor(getColorRRGGBB(brickX * 17, 0, 255 - brickX * 17), "GREEN"), get8bitColor(getColorRRGGBB(brickX * 17, 0, 255 - brickX * 17), "BLUE")));

      }
    }
    while (1) {
      oledc_fillRect(ballX / 10, ballY / 10, 2, 2, oledc_color565(get8bitColor(0xFFFFFF, "RED"), get8bitColor(0xFFFFFF, "GREEN"), get8bitColor(0xFFFFFF, "BLUE")));
      if (!input(4)) {
        paddleX--;
      }
      else if (!input(7)) {
        paddleX++;
      }
      paddleX = (constrainInt(paddleX, 2, 84));
      oledc_fillRect(paddleX, 61, paddleWidth, 2, oledc_color565(get8bitColor(0xffcc33, "RED"), get8bitColor(0xffcc33, "GREEN"), get8bitColor(0xffcc33, "BLUE")));
      pause(20);
      for (brickY = 0; brickY < 3; brickY++) {
        for (brickX = 0; brickX < 15; brickX++) {
          if (ballY <= ((brickY * 50) + 90) && ((ballX < (brickX * 60 + 58) && ballX >= (brickX * 60 - 2)) && bricks[(brickY * 15) + brickX])) {
            freqout(25, 2, 1000);
            oledc_fillRect(brickX * 6 + 3, brickY * 5 + 3, 5, 5, oledc_color565(get8bitColor(0x000000, "RED"), get8bitColor(0x000000, "GREEN"), get8bitColor(0x000000, "BLUE")));
            brickCount--;
            bricks[constrainInt((brickY * 15) + brickX, 0, 44)] = 0;
            ballSpeedY = -ballSpeedY;
          }


        }
      }
      if (!brickCount) {
        break;
      }

      if (ballX <= 40 || ballX >= 920) {
        ballSpeedX = -ballSpeedX;
      }

      if (ballY <= 40) {
        ballSpeedY = -ballSpeedY;
      }

      if (ballY >= 600) {
        paddleCenter = ((paddleX + (paddleWidth / 2)) * 10 - 1);
        if (ballX > (paddleX * 10) && ballX < ((paddleWidth + paddleX) * 10)) {
          ballSpeedX = (constrainInt((ballSpeedX + (ballX - paddleCenter)), -10, 10));
          ballSpeedY = -ballSpeedY;
        }
        else
        {
          break;
        }

      }

      oledc_fillRect(ballX / 10, ballY / 10, 2, 2, oledc_color565(get8bitColor(0x000000, "RED"), get8bitColor(0x000000, "GREEN"), get8bitColor(0x000000, "BLUE")));
      oledc_fillRect(paddleX, 61, paddleWidth, 2, oledc_color565(get8bitColor(0x000000, "RED"), get8bitColor(0x000000, "GREEN"), get8bitColor(0x000000, "BLUE")));
      ballX = (ballX + ballSpeedX);
      ballY = (ballY + ballSpeedY);
    }
  }

}
