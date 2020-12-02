// Conway's Game of Life //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define HALFW 120
#define HALFH 120
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

  uint16_t col[SCR];
  uint16_t coll;
  bool grid[2][HALFW][HALFH];
  int current;


void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);
  
  srand(read_cycle());

  for (int x = 0; x < HALFW; x++) {
    for (int y = 0; y < HALFH; y++) grid[0][x][y] = rand()%2;
  }

}

void loop()
{

  RunGrid();

  for (int y = 0; y < HALFH; y++) {
    for (int x = 0; x < HALFW; x++) {
      if(grid[current][x][y]) coll = COLOR_WHITE;
      else coll = COLOR_BLACK;
      col[(2*x)+(2*y)*WIDTH] = coll;
    }
  }

  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}

void RunGrid(){
  
  int x, y;
  int count;
  int value = 0;
  int new_grid;

  new_grid = 1 - current;
  
  for (y = 0; y < HALFH; y++) {
  
    for (x = 0; x < HALFW; x++) {
      
      count = Neighbours(x, y);
      
      if (count < 2 || count > 3) { value = 0; }
      else if (count == 3) { value = 3; }
      else { value = grid[current][x][y]; }
    
      grid[new_grid][x][y] = value;
    
    }
  }
  
  current = new_grid;
}


int Neighbours(int x, int y)
{
  int i, j;
  int result = 0;

  x--;
  y--;
  
  for (i = 0; i < 3; i++) {
  
    if (y < 0 || y > (HALFH - 1)) continue;

    for (j = 0; j < 3; j++) {
      if (x < 0 || x > (HALFW - 1)) continue;
      if (i==1 && j == 1) { x++; continue; }
      if (grid[current][x][y]) result++;
      x++;
    }
    y++;
    x -= 3;
  }
  
  return result;
} 
