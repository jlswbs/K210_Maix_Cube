// Hexagonal cellular automata //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)
#define ITER 192

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

  uint16_t col[SCR];

  int p[WIDTH][HEIGHT];
  int v[WIDTH][HEIGHT];

  int cnt;

void InitGrid(){

  memset(col,0,2*SCR);
  
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      p[x][y] = 1;
      v[x][y] = 1;
    }
  }

  p[WIDTH/2][HEIGHT/2] = -1;

}
  
void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);
  
  srand(read_cycle());

  InitGrid();
      
}

void loop(){

  if (cnt == ITER) {

    InitGrid();
    cnt = 0;
    
  } else { 

  for (int x = 1; x < WIDTH-1; x++) {
    for (int y = 1; y < HEIGHT-1; y+=2) {
      v[x][y] *= p[x][y] * p[x-1][y] * p[x+1][y] * p[x][y-1] * p[x][y+1] * p[x+1][y-1] * p[x+1][y+1];
    }
    for (int y = 2; y < HEIGHT-1; y+=2) {
      v[x][y] *= p[x][y] * p[x-1][y] * p[x+1][y] * p[x][y-1] * p[x][y+1] * p[x-1][y+1] * p[x-1][y-1];
    }
  }

  for (int y = 1; y < HEIGHT-1; y++) {
    for (int x = 1; x < WIDTH-1; x++) {
      p[x][y] *= v[x][y];
      if (p[x][y] == -1) col[x+y*WIDTH] = COLOR_WHITE;
      else col[x+y*WIDTH] = COLOR_BLACK;
    }
  }

  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

  }

  cnt++;

}
