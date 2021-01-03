// StarWars Cellular Automata //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

uint16_t RGB565(uint8_t R, uint8_t G, uint8_t B){ return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );}

  uint16_t col[SCR];
  uint16_t coll;
  
  int current [WIDTH][HEIGHT];
  int next [WIDTH][HEIGHT];
  int alive_counts [WIDTH][HEIGHT];
  int tmp[WIDTH][HEIGHT];
  
  int ALIVE = 3;
  int DEATH_1 = 2;
  int DEATH_2 = 1;
  int DEAD = 0;

  int x, y, nx, ny;


void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);

  srand(read_cycle());

  for (y = 0; y < HEIGHT; y++) { for (x = 0; x < WIDTH; x++) current[x][y] = random(0, 100) < 20 ? ALIVE : DEAD; }

}

void loop(){
  
  step();
      
  memset(col,0,2 * SCR);
  
  draw_type(50,100, 100,100,100);
  draw_type(2,49, 255,100,100);
  draw_type(0,1, 255,255,255); 
  
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}


void step(){
 
  for (x = 0; x < WIDTH; x++) {  
    for (y = 0; y < HEIGHT; y++) {
    
      int count = 0;
      int next_val;
    
      int mx = WIDTH-1;
      int my = HEIGHT-1;
    
      int self = current[x][y];
    
      for (nx = x-1; nx <= x+1; nx++) {
  
        for (ny = y-1; ny <= y+1; ny++) {
    
          if (nx == x && ny == y) continue;     
          if (current[wrap(nx, mx)][wrap(ny, my)] == ALIVE) count++;
      
        }   
      }  

    int neighbors = count;
    
    if (self == ALIVE) next_val = ((neighbors == 3) || (neighbors == 4) || (neighbors == 5)) ? ALIVE : DEATH_1;
  
    else if (self == DEAD) next_val = (neighbors == 2) ? ALIVE : DEAD;
  
    else next_val = self-1;
   
    next[x][y] = next_val;
  
    if (next_val == ALIVE) alive_counts[x][y] = min(alive_counts[x][y]+1, 100);
    else if (next_val == DEAD) alive_counts[x][y] = 0;
    
    }
  }
 
    for (x = 0; x < WIDTH; x++) { 
      for (y = 0; y < HEIGHT; y++) {
         
        tmp[x][y] = current[x][y];
        current[x][y] = next[x][y];
        next[x][y] = tmp[x][y];
    
    }
  } 
}
  
int wrap(int v, int m){

    if (v < 0) return v + m;
    else if (v >= m) return v - m;
    else return v;
}
  
void draw_type(int min_alive, int max_alive, uint8_t r, uint8_t g, uint8_t b){  
    
  for (x = 0; x < WIDTH; x++) {      
    for (y = 0; y < HEIGHT; y++) {

        int self = current[x][y];
        if (self == DEAD) continue;
   
        int alive = alive_counts[x][y];
        if (alive < min_alive || alive > max_alive) continue;
        
        if (self == ALIVE) coll = RGB565(r, g, b);
        else if (self == DEATH_1) coll = RGB565(r>>1, g>>1, b>>1);
        else if (self == DEATH_2) coll = RGB565(r>>2, g>>2, b>>2);
        
        col[x+y*WIDTH] = coll;
   
      }
   }
}
