// Belousov-Zhabotinsky reaction cellular automata //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

  uint16_t col[SCR];

  int cells[WIDTH][HEIGHT];
  int nextcells[WIDTH][HEIGHT];
  int dir[2][4] = {{0, 2, 0, -2},{-2, 0, 2, 0}};
  uint16_t coll;

  
void setup(){

  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);
  
  srand(read_cycle());
  
}

void loop(){
    
  for(int y = 0; y < HEIGHT; y=y+2){
    for(int x = 0; x < WIDTH; x=x+2){
      if(cells[x][y] == 0){
        coll = COLOR_BLACK;
      } else if(cells[x][y] == 1){
        coll = COLOR_RED;
      } else {
        coll = COLOR_WHITE;
      }
      col[x+y*WIDTH] = coll;
    }
  }

  for(int y = 0; y < HEIGHT; y=y+2){
    for(int x = 0; x < WIDTH; x=x+2){
      if(cells[x][y] == 1) nextcells[x][y] = 2;
      else if(cells[x][y] == 2) nextcells[x][y] = 0;
      else {
        nextcells[x][y] = 0;
        for(int i = 0; i < 4; i++){
          int dx = x + dir[0][i];
          int dy = y + dir[1][i];
          if(0 <= dx && dx < WIDTH && 0 <= dy && dy < HEIGHT && cells[dx][dy] == 1) nextcells[x][y] = 1;
        }
      }
    }
  }
  
  for(int y = 0; y < HEIGHT; y++){
    for(int x = 0; x < WIDTH; x++) {
      cells[x][y] = nextcells[x][y];
    }
  }
  
  cells[rand()%WIDTH][rand()%HEIGHT] = rand()%2;  
      
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
