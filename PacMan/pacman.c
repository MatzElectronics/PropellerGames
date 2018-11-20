// Pac-Man/Muncher-like game using 96x64 color OLED and pushbuttons on the
// Parallax Propeller MCU.
// By M.Matz, ideas for implementation from:
//    http://subethasoftware.com/2014/02/23/arduino-pac-man-part-10-the-story-so-far/#comment-29775
// Google Sheet for creating board layout array here:
//    https://docs.google.com/spreadsheets/d/1lRU5S3pfA02uIeb_-zMYhUdtndrubN-DYTTId55fP5o


// Define the pins the pushbuttons are connected to:
#define BUTTON_DOWN         06
#define BUTTON_UP           05
#define BUTTON_LEFT         04
#define BUTTON_RIGHT        07
#define BUTTON_RESTART      10

// Define the buttons the color OLED is connected to:
#define OLED_PIN_DIN        22
#define OLED_PIN_CLK        21
#define OLED_PIN_CS         20
#define OLED_PIN_DC         19
#define OLED_PIN_RES        18


#define STOPPED    0
#define LEFT       0b0100
#define DOWN       0b0001
#define RIGHT      0b1000
#define UP         0b0010
#define LIVES      3
#define TILESWIDE  28
#define TILESHIGH  18

#include "simpletools.h"                      // Include simple tools
#include "oledc.h"

const char gameBoard1[] = {3,7,7,7,7,7,7,7,7,7,7,7,7,4,3,7,7,7,7,7,7,7,7,7,7,7,7,4,8,1,1,1,1,1,1,1,1,1,1,1,1,8,8,1,1,1,1,1,1,1,1,1,1,1,1,8,8,1,3,7,7,4,1,3,4,1,3,7,7,5,6,7,7,4,1,3,4,1,3,7,7,4,1,8,8,1,6,7,7,5,1,8,8,1,6,7,7,7,7,7,7,5,1,8,8,1,6,7,7,5,1,8,8,2,1,1,1,1,1,8,8,0,0,0,0,0,0,0,0,0,0,8,8,1,1,1,1,1,2,8,6,7,7,7,7,4,1,8,8,0,15,10,10,10,10,10,10,16,0,8,8,1,3,7,7,7,7,5,7,7,7,7,7,5,1,8,8,0,14,9,9,9,9,9,9,13,0,8,8,1,6,7,7,7,7,7,0,0,0,0,0,0,1,8,8,0,14,9,9,9,9,9,9,13,0,8,8,1,0,0,0,0,0,0,7,7,7,7,7,4,1,8,8,0,14,9,9,9,9,9,9,13,0,8,8,1,3,7,7,7,7,7,3,7,7,7,7,5,1,6,5,0,18,12,12,12,12,12,12,17,0,6,5,1,6,7,7,7,7,4,8,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,8,8,1,3,7,7,4,1,3,7,7,7,4,1,3,4,1,3,7,7,7,4,1,3,7,7,4,1,8,8,1,6,7,4,8,1,6,7,7,7,5,1,8,8,1,6,7,7,7,5,1,8,3,7,5,1,8,8,2,1,1,8,8,1,1,1,1,1,1,1,8,8,1,1,1,1,1,1,1,8,8,1,1,2,8,6,7,4,1,8,8,1,3,4,1,3,7,7,5,6,7,7,4,1,3,4,1,8,8,1,3,7,5,3,7,5,1,6,5,1,8,8,1,6,7,7,7,7,7,7,5,1,8,8,1,6,5,1,6,7,4,8,1,1,1,1,1,1,8,8,1,1,1,1,1,1,1,1,1,1,8,8,1,1,1,1,1,1,8,6,7,7,7,7,7,7,5,6,7,7,7,7,7,7,7,7,7,7,5,6,7,7,7,7,7,7,5};
const char gameBoard2[] = {3,7,7,7,7,7,7,7,7,7,7,7,7,4,3,7,7,7,7,7,7,7,7,7,7,7,7,4,8,1,1,1,1,1,1,1,1,1,1,1,1,8,8,1,1,1,1,1,1,1,1,1,1,1,1,8,8,1,3,4,1,3,7,7,7,7,7,4,1,8,8,1,3,7,7,7,7,7,4,1,3,4,1,8,8,1,8,8,1,6,7,7,7,7,7,5,1,6,5,1,6,7,7,7,7,7,6,1,8,8,1,8,8,2,8,8,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,8,8,2,8,8,1,8,8,1,3,7,7,4,0,15,11,11,10,10,11,11,15,0,3,7,7,4,1,8,8,1,8,5,1,6,5,1,8,0,0,8,0,14,9,9,9,9,9,9,14,0,8,0,0,8,1,6,5,1,6,0,1,1,1,1,8,0,0,8,0,14,9,9,9,9,9,9,14,0,8,0,0,8,1,1,1,1,0,4,1,3,7,7,5,0,0,8,0,14,9,9,9,9,9,9,14,0,8,0,0,6,7,7,4,1,3,8,1,6,7,7,7,7,7,5,0,18,12,12,12,12,12,12,18,0,6,7,7,7,7,7,5,1,8,8,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,8,6,7,7,7,4,1,3,7,4,1,3,7,7,7,7,7,7,4,1,3,7,4,1,3,7,7,7,5,3,7,7,7,5,1,6,7,5,1,8,3,7,7,7,7,4,8,1,6,7,5,1,6,7,7,7,4,8,1,1,1,1,1,1,1,1,1,8,8,1,1,1,1,8,8,1,1,1,1,1,1,1,1,1,8,8,2,3,7,7,7,7,4,1,3,5,8,1,3,4,1,8,6,4,1,3,7,7,7,7,4,2,8,8,1,6,7,7,7,7,5,1,6,7,5,1,8,8,1,6,7,5,1,6,7,7,7,7,5,1,8,8,1,1,1,1,1,1,1,1,1,1,1,1,8,8,1,1,1,1,1,1,1,1,1,1,1,1,8,6,7,7,7,7,7,7,7,7,7,7,7,7,5,6,7,7,7,7,7,7,7,7,7,7,7,7,5};
char gameBoard[504];

int colorWall, colorDots, colorPill, colorFence, colorGate, colorPac, colorScore, colorZombie, colorGhost, colorG;
char pacDirection = STOPPED, pacState;
int pacLocation = 293;

int ghostLocation[4];
char ghostDirection[4];
int ghostZombieTimer[4];
int ghostStartTimer[4];
int theScore = 0;
int boardScore = 0;
signed char lives = -1;

int boardTurn = 1;

int gameDelay = 20;    // ms to slow things down

void drawBoard(void);
void setupGame(void);
void drawTile(int m);
void drawGhost(char g, char p);
void drawPac(void);
void canMove(void);
void ghostCanMove(char g);
void checkButtons(void);
void findBoardScore(void);
void drawGhost(char g, char p);
void reduceLives(void);
void drawBoardPart(int l);

int main()                                    
{
  
  ghostLocation[0] = 179;
  ghostDirection[0] = UP;
  ghostStartTimer[0] = 100;
  
  colorWall   = oledc_color565(0,0,128);
  colorDots   = oledc_color565(128,128,128);
  colorPill   = oledc_color565(200,200,128);
  colorFence  = oledc_color565(64,32,128);
  colorGate   = oledc_color565(32,32,64);
  colorPac    = oledc_color565(255,255,0);
  colorGhost  = oledc_color565(200,200,255);
  colorZombie = oledc_color565(0,0,200);
  colorScore  = oledc_color565(127,255,127);
  
  
  oledc_init(OLED_PIN_DIN,OLED_PIN_CLK,OLED_PIN_CS,OLED_PIN_DC,OLED_PIN_RES,2);
  oledc_setTextColor(colorScore, 0);


  while(1) {
    if (boardTurn == 1) {
      memcpy(gameBoard, gameBoard1, sizeof(gameBoard1));
      boardTurn++;
    } else if (boardTurn == 2){
      memcpy(gameBoard, gameBoard2, sizeof(gameBoard1));
      boardTurn++;
    }
    
    if (boardTurn == 3) {
      boardTurn = 1;
    }
    
    setupGame();
    findBoardScore();
    // draw black box first.
    drawBoard();
    drawPac();
    
    
    while(1)
    {
      checkButtons();
      drawPac();

      if(boardScore == 0) {
        ghostLocation[0] = 179;
        ghostDirection[0] = UP;
        ghostStartTimer[0] = 50;
        break;
      }        
      if(lives < 0) break;
    } 
    
    if(lives < 0) {
      oledc_setCursor(40, 56, 0);
      oledc_drawText("GAME OVER");
      while(input(BUTTON_RESTART));
      pause(100); 
    }      
  }   
}

void setupGame(void) {

  if(lives < 0) {
    lives = LIVES;
    theScore = 0;
  
    // clear the screen
    oledc_fillRect(0, 0, 97, 65, 0);
    
    // Show lives
    oledc_fillRect(79, 57, 4, 3, colorPac);
    oledc_fillRect(80, 56, 3, 5, colorPac);
    oledc_drawLine(81, 58, 83, 58, 0);
    oledc_drawLine(82, 57, 82, 59, 0);
    
    oledc_fillRect(73, 57, 4, 3, colorPac);
    oledc_fillRect(74, 56, 3, 5, colorPac);
    oledc_drawLine(75, 58, 77, 58, 0);
    oledc_drawLine(76, 57, 76, 59, 0);
    
    oledc_fillRect(67, 57, 4, 3, colorPac);
    oledc_fillRect(68, 56, 3, 5, colorPac);
    oledc_drawLine(81, 58, 83, 58, 0);
    oledc_drawLine(82, 57, 82, 59, 0);
    oledc_drawLine(69, 58, 71, 58, 0);
    oledc_drawLine(70, 57, 70, 59, 0);
  }
  
  boardScore = 0;  
  colorG = colorGhost;
}

void findBoardScore(void) {
  for(int j = 0; j < sizeof(gameBoard); j++) {
    if(gameBoard[j] == 1 || gameBoard[j] == 2) boardScore++;
  }
}

void checkButtons(void) {
  char oldDirection = pacDirection;
  if(!input(BUTTON_RIGHT)) pacDirection = RIGHT;
  if(!input(BUTTON_UP))    pacDirection = UP;
  if(!input(BUTTON_DOWN))  pacDirection = DOWN;
  if(!input(BUTTON_LEFT))  pacDirection = LEFT;
  
  canMove();
  if(pacDirection == STOPPED) pacDirection = oldDirection;
}

void drawBoardPart(int l) {
  print("bd=%d\r",l);
  drawTile(l - TILESWIDE + 1);
  drawTile(l - TILESWIDE);
  drawTile(l - TILESWIDE - 1);
  drawTile(l - 1);
  drawTile(l);
  drawTile(l + 1);
  drawTile(l + TILESWIDE + 1);
  drawTile(l + TILESWIDE);
  drawTile(l + TILESWIDE - 1);
}  

// assumes that the next tile to move to is safe - must call ghostCanMove(g) first.
void drawGhost(char g, char p) {
  int li = ghostLocation[g];
  
    char zx = li % TILESWIDE;
    char zy = li / TILESWIDE;
    
    int vx, vy, ds;
    
    switch(ghostDirection[g]) {
      case RIGHT:
        vx = 1; vy = 0;        
        break;
      case DOWN:
        vx = 0; vy = 1;
        break;
      case LEFT:
        vx = -1; vy = 0;
        break;
      case UP:
        vx = 0; vy = -1;
        break;
      case STOPPED:
        vx = 0; vy = 0;
        break;
    }    
  
  oledc_fillRect(zx*3 + vx*(p-1) - 1, zy*3 + vy*(p-1) - 1, 5, 5, 0);

  drawBoardPart(li);  
  
  oledc_drawLine(zx*3 + vx*p,     zy*3 + vy*p + 1, zx*3 + vx*p + 2, zy*3 + vy*p + 1, 0);
  
  oledc_drawLine(zx*3 + vx*p,     zy*3 + vy*p - 1, zx*3 + vx*p + 2, zy*3 + vy*p - 1, colorG);
  oledc_drawLine(zx*3 + vx*p - 1, zy*3 + vy*p + 1, zx*3 + vx*p + 3, zy*3 + vy*p + 1, colorG);
  oledc_drawLine(zx*3 + vx*p - 1, zy*3 + vy*p + 2, zx*3 + vx*p + 3, zy*3 + vy*p + 2, colorG);
  oledc_drawLine(zx*3 + vx*p - 1, zy*3 + vy*p    , zx*3 + vx*p - 1, zy*3 + vy*p + 3, colorG);
  oledc_drawLine(zx*3 + vx*p + 3, zy*3 + vy*p    , zx*3 + vx*p + 3, zy*3 + vy*p + 3, colorG);
  oledc_drawLine(zx*3 + vx*p + 1, zy*3 + vy*p    , zx*3 + vx*p + 1, zy*3 + vy*p + 3, colorG);
} 

void reduceLives(void) {
  lives--;
  if(lives >= 0) oledc_fillRect(67 + 6*lives, 56, 5, 5, 0);
}   
  
void drawPac(void) {
  
  canMove();
  
  char gx = pacLocation % TILESWIDE;
  char gy = pacLocation / TILESWIDE;
  
  int dx, dy, ds;
  
  switch(pacDirection) {
    case RIGHT:
      dx = 1; dy = 0;
      break;
    case DOWN:
      dx = 0; dy = 1;
      break;
    case LEFT:
      dx = -1; dy = 0;
      break;
    case UP:
      dx = 0; dy = -1;
      break;
    case STOPPED:
      dx = 0; dy = 0;
      break;
  }    
    
  oledc_fillRect( gx*3, gy*3 - 1, 3, 5, colorPac);
  oledc_fillRect( gx*3 - 1, gy*3, 5, 3, colorPac);
  
  ghostCanMove(0);
  drawGhost(0, 0);
  
  pause(gameDelay);
  
  // erase
  oledc_fillRect( gx*3 + dx*0 - 1, gy*3 + dy*0 - 1, 5, 5, 0);
  
  // re-draw
  oledc_fillRect( gx*3 + dx, gy*3 - 1 + dy, 3, 5, colorPac);
  oledc_fillRect( gx*3 - 1 + dx, gy*3 + dy, 5, 3, colorPac);
  
  if(pacDirection == RIGHT)  oledc_drawLine( gx*3 + 1 + dx, gy*3 + 1 + dy, gx*3 + 3 + dx, gy*3 + 1 + dy, 0);
  if(pacDirection == LEFT)   oledc_drawLine( gx*3 - 1 + dx, gy*3 + 1 + dy, gx*3 + 1 + dx, gy*3 + 1 + dy, 0);
  if(pacDirection == UP)     oledc_drawLine( gx*3 + 1 + dx, gy*3 - 1 + dy, gx*3 + 1 + dx, gy*3 + 1 + dy, 0);
  if(pacDirection == DOWN)   oledc_drawLine( gx*3 + 1 + dx, gy*3 + 1 + dy, gx*3 + 1 + dx, gy*3 + 3 + dy, 0);
  
  
  drawGhost(0, 1);
  
  pause(gameDelay);
  
  // erase
  oledc_fillRect( gx*3 + dx*1 - 1, gy*3 + dy*1 - 1, 5, 5, 0);
  
  // re-draw
  oledc_fillRect( gx*3 + dx*2, gy*3 - 1 + dy*2, 3, 5, colorPac);
  oledc_fillRect( gx*3 - 1 + dx*2, gy*3 + dy*2, 5, 3, colorPac);
  
  if(pacDirection == RIGHT) {
    oledc_fillRect( gx*3 + 2 + dx*2, gy*3 + dy*2, 2, 3, 0);
    oledc_drawLine( gx*3 + 1 + dx*2, gy*3 + 1 + dy*2, gx*3 + 3 + dx*2, gy*3 + 1 + dy*2, 0);
  } else if(pacDirection == LEFT) {
    oledc_fillRect( gx*3 - 1 + dx*2, gy*3 + dy*2, 2, 3, 0);
    oledc_drawLine( gx*3 - 1 + dx*2, gy*3 + 1 + dy*2, gx*3 + 1 + dx*2, gy*3 + 1 + dy*2, 0);
  } else if(pacDirection == UP) {
    oledc_fillRect( gx*3 + dx*2, gy*3 - 1 + dy*2, 3, 2, 0);
    oledc_drawLine( gx*3 + 1 + dx*2, gy*3 - 1 + dy*2, gx*3 + 1 + dx*2, gy*3 + 1 + dy*2, 0);
  } else if(pacDirection == DOWN) {
    oledc_fillRect( gx*3 + dx*2, gy*3 + 2 + dy*2, 3, 2, 0);
    oledc_drawLine( gx*3 + 1 + dx*2, gy*3 + 1 + dy*2, gx*3 + 1 + dx*2, gy*3 + 3 + dy*2, 0);
  }    
       
  
  drawGhost(0, 2);     
        
  pause(gameDelay);
  
  // erase
  oledc_fillRect( gx*3 + dx*2 - 1, gy*3 + dy*2 - 1, 5, 5, 0);
  
  // re-draw
  oledc_fillRect( gx*3 + dx*3, gy*3 - 1 + dy*3, 3, 5, colorPac);
  oledc_fillRect( gx*3 - 1 + dx*3, gy*3 + dy*3, 5, 3, colorPac);
  
  
  // set new location
  if(pacDirection == RIGHT) {
    if(pacLocation == 222) {
    // erase
    oledc_fillRect( gx*3 + dx*2 - 1, gy*3 + dy*2 - 1, 6, 5, 0);
    
    pacLocation = 196;
    
    gx = pacLocation % TILESWIDE;
    gy = pacLocation / TILESWIDE;

    oledc_fillRect( gx*3, gy*3 - 1, 3, 5, colorPac);
    oledc_fillRect( gx*3 - 1, gy*3, 5, 3, colorPac);
      
    } else pacLocation++;
  }      
  if(pacDirection == LEFT) {
    if(pacLocation == 197) {
    // erase
    oledc_fillRect( gx*3 + dx*2 - 1, gy*3 + dy*2 - 1, 5, 5, 0);
    
    pacLocation = 223;
    
    gx = pacLocation % TILESWIDE;
    gy = pacLocation / TILESWIDE;

    oledc_fillRect( gx*3, gy*3 - 1, 3, 5, colorPac );
    oledc_fillRect( gx*3 - 1, gy*3, 5, 3, colorPac );
      
    } else pacLocation--;
  }      
  if(pacDirection == DOWN) pacLocation += TILESWIDE;
  if(pacDirection == UP) pacLocation -= TILESWIDE;
  
  
  drawGhost(0, 3);

  if(ghostDirection[0] == DOWN)  ghostLocation[0] +=TILESWIDE;
  if(ghostDirection[0] == UP)    ghostLocation[0] -=TILESWIDE;
  if(ghostDirection[0] == RIGHT) ghostLocation[0]++;
  if(ghostDirection[0] == LEFT)  ghostLocation[0]--;
  ghostCanMove(0);
  
  if(gameBoard[pacLocation] == 2) {
    colorG = colorZombie;
    ghostZombieTimer[0] = 50;
  }
  
  char collision = 0;
  if(ghostDirection[0] == LEFT && pacDirection == RIGHT && pacLocation + 1 == ghostLocation[0]) collision = 1;
  if(ghostDirection[0] == RIGHT && pacDirection == LEFT && pacLocation - 1 == ghostLocation[0]) collision = 1;
  if(ghostDirection[0] == UP && pacDirection == DOWN && pacLocation + TILESWIDE == ghostLocation[0]) collision = 1;
  if(ghostDirection[0] == DOWN && pacDirection == UP && pacLocation - TILESWIDE == ghostLocation[0]) collision = 1;
  if(pacLocation == ghostLocation[0]) collision = 1;
  
  if(colorG == colorZombie) {
    ghostZombieTimer[0]--;
    if(collision == 1) {
      int li = ghostLocation[0];
      drawBoardPart(li);  

      ghostLocation[0] = 179;
      ghostDirection[0] = UP;
      ghostStartTimer[0] = 50;

      theScore += 200;
      colorG = colorGhost;
      
    }      
  } else {
    if(collision == 1) {
      int li = ghostLocation[0];
      drawBoardPart(li);  

      ghostLocation[0] = 179;
      ghostDirection[0] = UP;
      ghostStartTimer[0] = 50;

      oledc_fillRect((pacLocation % TILESWIDE)*3-1, (pacLocation / TILESWIDE)*3-1, 5, 5, 0);
      pacLocation = 293;
      reduceLives();
    }
  }

  if(ghostZombieTimer[0] <= 0) {
    colorG = colorGhost;
  }

  // erase dot, add to score if there was a dot here
  if(gameBoard[pacLocation] == 1 || gameBoard[pacLocation] == 2) {
    gameBoard[pacLocation] = 0;
    theScore++;
    boardScore--;
  }      

  oledc_setCursor(0,56,0);
  oledc_drawNumber(theScore, DEC);
}  

void canMove(void) {
  int c = 0;
  if(pacDirection == LEFT && pacLocation == 223) c = 1;
  else if(pacDirection == RIGHT && pacLocation == 196) c = 1;
  else if(pacDirection == RIGHT && gameBoard[pacLocation + 1] < 3) c = 1;
  else if(pacDirection == LEFT && gameBoard[pacLocation - 1] < 3) c = 1;
  else if(pacDirection == DOWN && gameBoard[pacLocation + TILESWIDE] < 3) c = 1;
  else if(pacDirection == UP && gameBoard[pacLocation - TILESWIDE] < 3) c = 1;
  
  if(c == 0) pacDirection = STOPPED;
}  

void ghostCanMove(char g) {
  char c = 0;
  char d = 0;
  
  char gateOpen = 10;  // switch to 10 when the gate opens
  if(ghostStartTimer[g] > 0) {
    gateOpen = 9;
    ghostStartTimer[g]--;
  }    

  // can continue?
  if(gameBoard[ghostLocation[g] + 1] < 3  || gameBoard[ghostLocation[g] + 1] == 9  || gameBoard[ghostLocation[g] + 1] == gateOpen)  { c |= RIGHT; d++; }
  if(gameBoard[ghostLocation[g] - 1] < 3  || gameBoard[ghostLocation[g] - 1] == 9  || gameBoard[ghostLocation[g] - 1] == gateOpen)  { c |= LEFT; d++; }
  if(gameBoard[ghostLocation[g] + TILESWIDE] < 3 || gameBoard[ghostLocation[g] + TILESWIDE] == 9 || gameBoard[ghostLocation[g] + TILESWIDE] == gateOpen) { c |= DOWN; d++; }
  if(gameBoard[ghostLocation[g] - TILESWIDE] < 3 || gameBoard[ghostLocation[g] - TILESWIDE] == 9 || gameBoard[ghostLocation[g] - TILESWIDE] == gateOpen) { c |= UP; d++; }

  if(!(c & ghostDirection[g]) || d > 2) {
  // must change directions

    int gh = ghostLocation[g] % TILESWIDE;
    int ph = pacLocation % TILESWIDE;
    int gv = ghostLocation[g] / TILESWIDE;
    int pv = pacLocation / TILESWIDE;

    int tempDist = TILESWIDE * 2;
    int tempDir = STOPPED;
  
    // check each direction, move toward (or away from) the player
    if(c & UP) {
      if(pv - gv <= tempDist && colorG == colorGhost) {
        tempDist = pv - gv;
        tempDir = UP;
      }
      if(gv - pv <= tempDist && colorG == colorZombie) {
        tempDist = gv - pv;
        tempDir = UP;
      }        
        
    }
    if(c & DOWN) {
      if(gv - pv <= tempDist && colorG == colorGhost) {
        tempDist = gv - pv;
        tempDir = DOWN;
      }        
      if(pv - gv <= tempDist && colorG == colorZombie) {
        tempDist = pv - gv;
        tempDir = DOWN;
      }        
    }
    
    if(c & RIGHT && gateOpen == 10) {
      if(gh - ph <= tempDist && colorG == colorGhost) {
        tempDist = gh - ph;
        tempDir = RIGHT;
      }        
      if(ph - gh <= tempDist && colorG == colorZombie) {
        tempDist = ph - gh;
        tempDir = RIGHT;
      }        
    }
    if(c & LEFT && gateOpen == 10) {
      if(ph - gh <= tempDist && colorG == colorGhost) {
        tempDist = ph - gh;
        tempDir = LEFT;
      }        
      if(gh - ph <= tempDist && colorG == colorZombie) {
        tempDist = gh - ph;
        tempDir = LEFT;
      }        
    }
    
    if(ghostStartTimer[0] <= 0 && (gameBoard[ghostLocation[g]] == 9 || gameBoard[ghostLocation[g]] == 10)) tempDir = UP;
    
    ghostDirection[g] = tempDir;      
  }   
}  

void drawBoard(void) {
  oledc_clear(0, 0, TILESWIDE * 3, TILESHIGH * 3);
  for( char k = 0; k < TILESWIDE; k++ ) {
    for ( char l = 0; l < TILESHIGH; l++ ) {
      int m = k + ( l * TILESWIDE );
      drawTile(m);
    }
  }    
}

void drawTile(int m) { 
  if(m >= 0 || m <= sizeof(gameBoard)) {
    int k = m % TILESWIDE;
    int l = m / TILESWIDE;

    switch( gameBoard[m] ) {
      case 0:   // blank tile
        oledc_fillRect( k*3-1, l*3-1, 5, 5, 0 );
        break;      
      case 1:   // the normal dot
        oledc_drawPixel( k*3 + 1, l*3 + 1, colorDots );
        break;
      case 2:   // the big dot
        oledc_drawLine( k*3, l*3 + 1, k*3 + 2, l*3 + 1, colorPill );          
        oledc_drawLine( k*3 + 1, l*3, k*3 + 1, l*3 + 2, colorPill ); 
        break;
      case 3:   // wall - top right corner
        oledc_drawPixel( k*3 + 2, l*3 + 1, colorWall );         
        oledc_drawPixel( k*3 + 1, l*3 + 2, colorWall );         
        break;
      case 4:   // wall - top left corner
        oledc_drawPixel( k*3 + 0, l*3 + 1, colorWall );         
        oledc_drawPixel( k*3 + 1, l*3 + 2, colorWall );         
        break;
      case 5:   // wall - bottom left corner
        oledc_drawPixel( k*3 + 1, l*3 + 0, colorWall );         
        oledc_drawPixel( k*3 + 0, l*3 + 1, colorWall );         
        break;
      case 6:   // wall - bottom right corner
        oledc_drawPixel( k*3 + 1, l*3 + 0, colorWall );         
        oledc_drawPixel( k*3 + 2, l*3 + 1, colorWall );         
        break;
      case 7:   // wall - vertical piece
        oledc_drawLine( k*3, l*3 + 1, k*3 + 2, l*3 + 1, colorWall );          
        break;
      case 8:   // wall - horizontal piece
        oledc_drawLine( k*3 + 1, l*3, k*3 + 1, l*3 + 2, colorWall ); 
        break;
      case 9:   // gohst pen - blank tile
        oledc_fillRect( k*3-1, l*3-1, 5, 5, 0 );
        break;  
      case 10:  // ghost pen - top gate
        oledc_drawLine( k*3, l*3 + 1, k*3 + 2, l*3 + 1, colorGate ); 
        break;
      case 11:  // ghost pen - 
      case 12:  
        oledc_drawLine( k*3, l*3 + 1, k*3 + 2, l*3 + 1, colorFence ); 
        break;
      case 13:  // ghost pen - 
      case 14:  
        oledc_drawLine( k*3 + 1, l*3, k*3 + 1, l*3 + 2, colorFence ); 
        break;
      case 15:  // ghost pen - 
        oledc_drawLine( k*3 + 1, l*3 + 1, k*3 + 1, l*3 + 2, colorFence ); 
        oledc_drawPixel( k*3 + 2, l*3 + 1, colorFence ); 
        break;
      case 16:  // ghost pen - 
        oledc_drawLine( k*3, l*3 + 1, k*3 + 1, l*3 + 1, colorFence ); 
        oledc_drawPixel( k*3 + 1, l*3 + 2, colorFence ); 
        break;
      case 17:  // ghost pen - 
        oledc_drawLine( k*3, l*3 + 1, k*3 + 1, l*3 + 1, colorFence ); 
        oledc_drawPixel( k*3 + 1, l*3 + 0, colorFence ); 
        break;
      case 18:  // ghost pen - 
        oledc_drawLine( k*3 + 1, l*3, k*3 + 1, l*3 + 1, colorFence ); 
        oledc_drawPixel( k*3 + 2, l*3 + 1, colorFence ); 
        break;
    }                
  } 
}
