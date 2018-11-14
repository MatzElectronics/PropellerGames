// ------ Libraries and Definitions ------
#include "simpletools.h"
#include "oledc.h"
#include "colormath.h"

// ------ Global Variables and Objects ------
int gameSpeed;
int pipeGap;
int pipeHeight;
int birdPower;
int loc;
int acceleration;
int pipeWidth;
int birdPosition;
int btnSet;
int score;
int birdSpeed;

// ------ Function Declarations ------
void setup_game();

// ------ Main Program ------
int main() {
  oledc_init(22, 21, 20, 19, 18, 2);
  setup_game();
  while (1) {
    pipeHeight = (random(0, ((oledc_getHeight()) - pipeGap)));
    for (loc = 0; loc < (oledc_getWidth()) + 8; loc++) {
      oledc_fillRect((oledc_getWidth()) - loc, 11, pipeWidth, pipeHeight, oledc_color565(0x33, 0xCC, 0));
      oledc_fillRect((oledc_getWidth()) - loc, pipeHeight + pipeGap, pipeWidth, oledc_getHeight(), oledc_color565(0x33, 0xCC, 0));
      if (!input(4) && !btnSet) {
        btnSet = 1;
        birdSpeed = birdPower;
      }

      if (input(4) && btnSet) {
        btnSet = 0;
      }

      birdPosition = birdPosition - birdSpeed / 10;
      birdSpeed = birdSpeed - acceleration;
      birdPosition = constrainInt(birdPosition, 11, oledc_getHeight());
      oledc_fillRect(10, birdPosition, 5, 5, oledc_color565(255, 255, 0));
      if (loc < oledc_getWidth() - 2 && loc > oledc_getWidth() - 10) {
        if (birdPosition < pipeHeight + 10 || birdPosition > pipeHeight + pipeGap - 5) {
          oledc_setTextColor(oledc_color565(255,255,255), oledc_color565(0xCC, 0, 0));
          oledc_setCursor(10, 20, 0);
          oledc_drawText("Game Over");
          while (input(4));
          pause(250);
          oledc_clear(0, 0, oledc_getWidth(), oledc_getHeight());
          setup_game();
          break;
        }

      }

      if (loc == oledc_getWidth() - 2) {
        score++;
        oledc_setCursor(1, 1, 0);
        oledc_print("%s%d%c", "Score = ", score, 32);
      }

      pause(gameSpeed);
      oledc_fillRect(0, 11, oledc_getWidth(), oledc_getHeight() - 11, 0);

    }
  }

}

// ------ Functions ------
void setup_game() {
  gameSpeed = 25;
  pipeGap = 17;
  birdPower = 30;
  acceleration = 4;
  pipeWidth = 8;
  birdPosition = 40;
  btnSet = 0;
  score = 0;
  pipeGap = (pipeGap + 11);
  birdSpeed = birdPower;
  oledc_setTextColor(oledc_color565(255,255,255), oledc_color565(0,0,0x99));
  oledc_fillRect(0, 0, oledc_getWidth(), 10, oledc_color565(0,0,0x99));
}
