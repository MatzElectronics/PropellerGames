/*
  Shooter.c
  A simple asteroids-like demonstration
  
*/

#include "simpletools.h"                     // Include simpletools library

#include "oledc.h"                      // Include badgetools library

typedef struct
{
  signed char dist, ang;
} SHOT;


typedef struct
{
  signed char offset_x, offset_y, hits_left, force_x, force_y;
} ASTEROID;


typedef struct
{
  signed char x1, x2, x3, x4, x5, x6;
} ASTEROIDX;


typedef struct
{
  signed char y1, y2, y3, y4, y5, y6;
} ASTEROIDY;


// Function declarations, so the compiler knows these are coming
void Init(void);
void GameLoop(void);
void Play(void);
void Draw(void);
void NewAsteroid(char j);
void ShipIsHit(char k);


//Constant values for various things about the game
const int Height = 32;      // Height of half the play area
const int Width = 48;       // Width of half the play area
const int VCenter = 33;     // Vertical center of the play area
const int HCenter = 48;     // Horizontal center of the play area
const int RotSpeed = 128;   // Rotation step for the ship when turning

#define  MaxShots  4        // maximum number of shots in flight at once
#define  ShotLen   2        // length in pixels of the shot
#define  ShotDelay 5        // updates before you can shoot again

#define  AsteroidCount     3
#define  AsteroidHits      3
#define  AsteroidDelay     15


int ShipAng = 0;            // Current direction of the ship

SHOT Shots[MaxShots];       // Array of SHOT structs, hold information about shots in-flight
char FreeShots = MaxShots;  // How many shots there are in the Shots array
char UsedShots = 0;         // How many we've used
char ShotTimer = 0;         // How many are left (this didn't have to be two variables, but it's easier to read this way)
char delay_a = 0;

ASTEROID Asteroid[AsteroidCount];
ASTEROIDX AsteroidX[AsteroidCount];
ASTEROIDY AsteroidY[AsteroidCount];


int ShipColor, ShotColor, AsteroidColor;

int AsteroidClock = 0;
int theScore;

char GameOver = 0;


void main()                   // Main function (standard C entry point)
{
  oledc_init(22,21,20,19,18,2);              // Call badge setup

  Init();                     // Initialize the game board
  GameLoop();                 // Run the game loop
}


// Set up some global things
void Init(void)
{
  GameOver = 0;
  theScore = 0;
  srand(INA + CNT);
  ShipColor = oledc_color565(0,0,255);
  ShotColor = oledc_color565(255,0,0);
  AsteroidColor = oledc_color565(0,255,0);
  
  UsedShots = 0;        // No shots currently active
  FreeShots = MaxShots;
  
  for(char k = 0; k < AsteroidCount; k++) NewAsteroid(k);  
  

}


void GameLoop(void)
{
  while(1)
  {
    while(1)     // Repeat forever
    { 
      Play();     // Update the game state
      Draw();     // Draw the game board
  
      pause(15);  // Slow the frame rate a little
    
      if(GameOver == 1) break;
    }
    oledc_setCursor(20,30,0);
    oledc_drawText("Game  Over");
    low(26);
    low(27);
    while(input(10));
    Init();
    pause(100);
  }  
}


// Use the built-in SINE table in the Propeller to return a Sin value from -0xffff to +0xffff
// for angle values in the range of 0x0000 to 0x1fff being 0 to 360 degrees.  Values outside of
// this range are allowed, and will just wrap around.
int Sin(int a)
{
  if( a & 0x800 ) {
    a = 0x1000 - a;
  }
  int s = ((unsigned short *)0xe000)[a & 0xfff];
  if( a & 0x1000 ) {
    s = -s;
  }
  return s;    
}

// Return a cosine value in the same way (and same ranges) as Sin() above
int Cos(int a)
{
  return Sin( a + 0x800 );
}

// Rotate an input vector (coordinate) around the origin by the angle (a)
// stores the result in the variables pointed to by nx and ny
// The angle (a) has a range of 0 to 0x1fff (8191).  Values outside this
// range are allowed, but will simply wrap around.  All inputs are integers
// and the math is done using fixed-point multiplication with rounding.

void Rotate(int x, int y, int a, int * nx, int * ny)
{
  int cs = Cos(a), sn = Sin(a);
  *nx = ((cs * x) - (sn * y) + 32768) / 65536;
  *ny = ((sn * x) + (cs * y) + 32768) / 65536;
}


// Draw a Ship (ok, it's really just a triangle)

void DrawShip(void)
{
  // These are the coordinates of the ship
  int xc[] = { 0,  4,  0, -4};
  int yc[] = {-5,  5,  3,  5};

  int x[4], y[4];
  for( int i=0; i<4; i++ )
  {
    // rotate the input coordinates of the ship to the correct orienation
    Rotate( xc[i], yc[i], ShipAng, &x[i], &y[i] );

    // add the offset for the center of the screen to position it in the middle of the display
    x[i] += HCenter;
    y[i] += VCenter;
  }

  // Draw the lines connecting the rotated result points
  oledc_drawLine( x[0], y[0], x[1], y[1], ShipColor );
  oledc_drawLine( x[1], y[1], x[2], y[2], ShipColor );
  oledc_drawLine( x[2], y[2], x[3], y[3], ShipColor );
  oledc_drawLine( x[3], y[3], x[0], y[0], ShipColor );
}

void NewAsteroid(char j) {

  char startSide = rand() & 3;
  if(startSide == 0) {
    Asteroid[j].offset_x = 0;
    Asteroid[j].offset_y = 0 + (rand() & (2 * Height));
  } else if(startSide == 1) {
    Asteroid[j].offset_x = 2 * Width;
    Asteroid[j].offset_y = 0 + (rand() & (2 * Height));
  } else if(startSide == 2) {
    Asteroid[j].offset_y = 0;
    Asteroid[j].offset_x = 0 + (rand() & (2 * Width));
  } else if(startSide == 3) {
    Asteroid[j].offset_y = 2 * Height;
    Asteroid[j].offset_x = 0 + (rand() & (2 * Width));
  }
  
  Asteroid[j].hits_left = AsteroidHits;
        
        
  AsteroidX[j].x1 = -2 + (rand() & 4);
  AsteroidY[j].y1 =  8 + (rand() & 4);
  
  AsteroidX[j].x2 =  5 + (rand() & 4);
  AsteroidY[j].y2 =  3 + (rand() & 4);
  
  AsteroidX[j].x3 =  5 + (rand() & 4);
  AsteroidY[j].y3 = -7 + (rand() & 4);
  
  AsteroidX[j].x4 = -2 + (rand() & 4);
  AsteroidY[j].y4 =-12 + (rand() & 4);
  
  AsteroidX[j].x5 = -9 + (rand() & 4);
  AsteroidY[j].y5 = -7 + (rand() & 4);
  
  AsteroidX[j].x6 = -9 + (rand() & 4);
  AsteroidY[j].y6 =  3 + (rand() & 4);
}  

void MoveAsteroid(char j) {
  signed char x_dist = Asteroid[j].offset_x - HCenter;
  signed char y_dist = Asteroid[j].offset_y - VCenter;
  if(Asteroid[j].force_y != 1 && Asteroid[j].force_x != 1) {
    if(abs(x_dist) > abs(y_dist)) {
      if(Asteroid[j].force_y < 2) Asteroid[j].force_y = abs(x_dist)/abs(y_dist) + 1;
      x_dist < 0 ? Asteroid[j].offset_x++ : Asteroid[j].offset_x--;
    } else {
      if(Asteroid[j].force_x < 2) Asteroid[j].force_x = abs(y_dist)/abs(x_dist) + 1;
      y_dist < 0 ? Asteroid[j].offset_y++ : Asteroid[j].offset_y--;
    }     
  } else if(Asteroid[j].force_y == 1) {
    y_dist < 0 ? Asteroid[j].offset_y++ : Asteroid[j].offset_y--;
  } else {
    x_dist < 0 ? Asteroid[j].offset_x++ : Asteroid[j].offset_x--;
  }     
  Asteroid[j].force_x--;
  Asteroid[j].force_y--;         
}  

void DrawAsteroid(char j) {
  signed char ox = Asteroid[j].offset_x;
  signed char oy = Asteroid[j].offset_y;
  
  oledc_drawLine( AsteroidX[j].x1 + ox, AsteroidY[j].y1 + oy, AsteroidX[j].x2 + ox, AsteroidY[j].y2 + oy, AsteroidColor );
  oledc_drawLine( AsteroidX[j].x2 + ox, AsteroidY[j].y2 + oy, AsteroidX[j].x3 + ox, AsteroidY[j].y3 + oy, AsteroidColor );
  oledc_drawLine( AsteroidX[j].x3 + ox, AsteroidY[j].y3 + oy, AsteroidX[j].x4 + ox, AsteroidY[j].y4 + oy, AsteroidColor );
  oledc_drawLine( AsteroidX[j].x4 + ox, AsteroidY[j].y4 + oy, AsteroidX[j].x5 + ox, AsteroidY[j].y5 + oy, AsteroidColor );
  oledc_drawLine( AsteroidX[j].x5 + ox, AsteroidY[j].y5 + oy, AsteroidX[j].x6 + ox, AsteroidY[j].y6 + oy, AsteroidColor );
  oledc_drawLine( AsteroidX[j].x6 + ox, AsteroidY[j].y6 + oy, AsteroidX[j].x1 + ox, AsteroidY[j].y1 + oy, AsteroidColor );
}  

void ShrinkAsteroid(char j) {
                      AsteroidY[j].y1 -= 2;
  AsteroidX[j].x2--;  AsteroidY[j].y2--;
  AsteroidX[j].x3--;  AsteroidY[j].y3++;
                      AsteroidY[j].y4 += 2;
  AsteroidX[j].x5++;  AsteroidY[j].y5++;
  AsteroidX[j].x6++;  AsteroidY[j].y6--;
  
  Asteroid[j].hits_left--;
}  

char AsteroidIsHit(char k, int testx, int testy) {
  char c = 0;
  char bounds = Asteroid[k].hits_left * 3 + 2;
  
  if( abs(Asteroid[k].offset_x - testx) < bounds && abs(Asteroid[k].offset_y - testy) < bounds ) c = 1;
  
  return c;
}  

void ShipIsHit(char k) {
  char bounds = Asteroid[k].hits_left * 3 + 2;
  
  if( abs(Asteroid[k].offset_x - HCenter) < bounds && abs(Asteroid[k].offset_y - VCenter) < bounds && Asteroid[k].hits_left > 0) GameOver = 1;  
}

void Shoot(void)
{
  // Make sure we have a free slot to store the shot info in, and we're allowed to shoot (timer is zero)
  if( FreeShots == 0 || ShotTimer != 0 ) return;

  // This shot starts at the center of the screen
  Shots[UsedShots].dist = 0;

  // Since the ship doesn't use all the precision for what angle it's at, we don't need to
  // store that for the shots, either.  I chop off the bottom 7 bits so I can fit it into a char
  Shots[UsedShots].ang = ShipAng >> 7;

  // Keep track of how many shots are active, and how many free ones are available
  UsedShots++;
  FreeShots--;

  // Set a timer so we can't shoot again immediately
  ShotTimer = ShotDelay;
}


// This is probably the most complicated thing in the code.  In order to simplify storing the shots,
// and make their movement reasonably accurate, I just store how far from the center they are (dist)
// and what angle they were fired at.  The only thing to "move" is the distance, but it makes checking
// to see if the shot has gone off the playfield a little harder, because we have to rotate each shot
// by the angle and figure out if the result is off the display.

void MoveShot( int i )
{
  // We're going to test if the far end of the shot is off the screen yet (that's why we add ShotLen)
  int dist = Shots[i].dist + ShotLen + 1;

  int a = Shots[i].ang << 7;  // scale the angle value back up from when we stored it

  // Compute the rotated location of the shot
  int x = ( Sin(a) * dist + 32768) / 65536;
  int y = (-Cos(a) * dist + 32768) / 65536;

  // If the rotated x or y are outside the range of the display, the shot has gone off the screen
  if( abs(x) >= Width || abs(y) >= Height )
  {
    // out of bounds - remove this shot from the active list
    UsedShots--;
    FreeShots++;

    // Do we still have any shots that are active?
    if( UsedShots )
    {
      // Copy the data for the LAST active shot into this spot in the shots array
      // This compacts the list.
      memcpy( Shots + i, Shots + UsedShots, sizeof(SHOT) );

      // Because we just moved the LAST shot in the list to this spot in the Shots array,
      // the moved entry would get skipped by the updating code, because it has no idea
      // that we changed the list order.  We call MoveShot AGAIN, on the updated entry, so it doesn't get missed.
      // This is overkill for a simple demo, but it's good to know that this kind of thing happens.  :)
      MoveShot( i );
    }
  }
  else
  {
    Shots[i].dist = dist;
  }
}

// Iterate through the list of currently active shots and move them
void MoveShots(void)
{
  for( int i=0; i<UsedShots; i++ ) {
    MoveShot(i);
  }
}

// Iterate through the list of active shots and draw them
void DrawShots(void)
{
  for( int i=0; i<UsedShots; i++ )
  {
    int x0, y0, x1, y1;
    int d0 = Shots[i].dist;
    int d1 = d0 + ShotLen;

    int a = Shots[i].ang << 7;
    int cs = -Cos(a), sn = Sin(a);

    x0 = ((sn * d0) + 32768) / 65536 + HCenter;
    y0 = ((cs * d0) + 32768) / 65536 + VCenter;
    x1 = ((sn * d1) + 32768) / 65536 + HCenter;
    y1 = ((cs * d1) + 32768) / 65536 + VCenter;

    oledc_drawLine( x0, y0, x1, y1, ShotColor );
    
    for(char k = 0; k < AsteroidCount; k++) {
      if(AsteroidIsHit(k, x1, y1) == 1) {
        ShrinkAsteroid(k);
  
        UsedShots--;
        FreeShots++;
    
        // Do we still have any shots that are active?
        if( UsedShots )
        {
          // Copy the data for the LAST active shot into this spot in the shots array
          // This compacts the list.
          memcpy( Shots + i, Shots + UsedShots, sizeof(SHOT) );
    
          // Because we just moved the LAST shot in the list to this spot in the Shots array,
          // the moved entry would get skipped by the updating code, because it has no idea
          // that we changed the list order.  We call MoveShot AGAIN, on the updated entry, so it doesn't get missed.
          // This is overkill for a simple demo, but it's good to know that this kind of thing happens.  :)
          MoveShot( i );
        }
        break;
      }
    } 
               
  }
}


// Get player input, rotate the ship, fire shots, move active shots
void Play(void)
{
  
  if( !(input(14)) )
  {
    ShipAng = (ShipAng + RotSpeed) & 0x1fff;
    high(26);
  } else low(26);

  if( !(input(12)) )
  {
    ShipAng = (ShipAng - RotSpeed) & 0x1fff;
    high(27);
  } else low(27);


  if( ShotTimer > 0 ) {
    ShotTimer--;
  }    

  if( !(input(11)) ) {
    Shoot();
  }


  if(delay_a == 0) {
    if( AsteroidDelay > theScore/3 ) delay_a = AsteroidDelay - (theScore/3);
    else delay_a = 1;
    for(char k = 0; k < AsteroidCount; k++) if(Asteroid[k].hits_left > 0) MoveAsteroid(k);
  } else {
    delay_a--;
  }
  

  MoveShots();
}

// Draw our little universe
void Draw(void)
{
  oledc_fillRect(0,0,97,65,0);

  DrawShip();
  DrawShots();
     
  for(char k = 0; k < AsteroidCount; k++) {
    if(Asteroid[k].hits_left > 0) 
    {
      DrawAsteroid(k);
      ShipIsHit(k);
    }      
     
    if(Asteroid[k].hits_left == 0) {
      theScore++;
      NewAsteroid(k);
    } 
  }  
    
  oledc_setCursor(0,56,0);
  oledc_drawNumber(theScore, DEC);

}
