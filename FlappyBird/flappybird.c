#include "simpletools.h"                      // Include simple tools
#include "oledc.h"


int main() {                                  // Main function

  oledc_init(7, 6, 9, 8, 5, 1); // pins: CS, D/C, DIN, CLK, RES   rotation: 0 (pins upward)

  srand(CNT + INA);

  int speed = 75;
  int bird = 15;
  int btnSet = 0;
  int theScore = 0;
  int collision = 0;

  oledc_fillRect(0,0,oledc_getWidth(),17,NAVYBLUE);

  while(1) {
    oledc_setCursor(0,0,0);
    oledc_setTextColor(WHITE, WHITE);
    oledc_setTextSize(2);  
    
    int pipeHeight = rand() & 30 - 15;
    for(int d = 0; d < oledc_getWidth()+7; d++) {
      //draw pipes
      int pipeWidth = 8;
      if(d > oledc_getWidth()) {
        pipeWidth = 8 - d + oledc_getWidth();
      }
      oledc_fillRect(oledc_getWidth()-d,18,pipeWidth,28 + pipeHeight,oledc_color565(0,255,0));
      oledc_fillRect(oledc_getWidth()-d,oledc_getHeight()-(28 - pipeHeight),pipeWidth,28 - pipeHeight,oledc_color565(0,255,0));
      

      //check for a button press
      if(input(2) == 0 && btnSet == 0) {
        btnSet = 1;
        speed = 32;
      }
      
      //button debounce
      if(input(2) == 1 && btnSet == 1) btnSet = 0;
      
      //set bird's position        
      if(bird < 0) {
        bird = 0;
      }        
      bird = bird + speed / 10;
      speed = speed - 6;
      if(bird > oledc_getHeight()-18) {
        bird = oledc_getHeight()-18;
      }
      
      //did they score?
      if(d == oledc_getWidth() - 2) {
        theScore++;
        oledc_fillRect(0,0,oledc_getWidth(),17,NAVYBLUE);
        oledc_print(" %d", theScore);
      }        
      
      
      //draw bird
      oledc_fillRect(10,oledc_getHeight() - bird,5,5,YELLOWORANGE);
      
      //test for crash
      if(d >= (oledc_getWidth() - 10) && d <= (oledc_getWidth() - 2)) {
        if(bird < 28 + 5 - pipeHeight || bird > (oledc_getHeight() - 28 - 18 - pipeHeight)) {
          collision = 1;
          break;
        }
      }                  
      
      //wait, then erase pipes and bird
      pause(30);
      oledc_fillRect(10,oledc_getHeight() - bird,5,5,BLACK);
      oledc_fillRect(oledc_getWidth()-d,18,pipeWidth,28 + pipeHeight,BLACK);
      oledc_fillRect(oledc_getWidth()-d,oledc_getHeight()-(28 - pipeHeight),pipeWidth,28 - pipeHeight,BLACK);
    }    
    
    //was there a collision?
    if(collision) {
      //print "Game Over" on the screen
      oledc_setCursor(0,2,2);
      oledc_setTextColor(WHITE,DARKRED);
      oledc_print("Game \nOver ");
      
      //wait for a button press
      while(input(2));
      pause(250);
      
      //reset all of the variables
      speed = 75;
      bird = 15;
      btnSet = 0;
      theScore = 0;
      collision = 0;
    
      //clear the screen and replace the top dark blue box
      oledc_clear(0,0,oledc_getWidth(),oledc_getHeight());
      oledc_fillRect(0,0,oledc_getWidth(),17,NAVYBLUE);
    }      
  }    
}
