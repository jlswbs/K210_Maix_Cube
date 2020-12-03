// Simple Diffusion-Limited Aggregation //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

float randomf(float minf, float maxf) {return minf + (rand()%(1UL << 31))*(maxf - minf) / (1UL << 31);}
  
  uint16_t col[SCR];
  uint16_t coll = COLOR_WHITE;
  int x, y;
  
void setup()
{
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);
  
  srand(read_cycle());

  y = rand()%HEIGHT;
  x = rand()%WIDTH;
  col[x*WIDTH+y] = coll;
  
}

void loop(){
  
  for (int i=0;i<100;i++) {

    int r = rand()%WIDTH;
    x = 120+(WIDTH/2.1)*sin(r);
    y = 120+(HEIGHT/2.1)*cos(r);
    
    while (x>2 && x<WIDTH-2 && y>2 && y<HEIGHT-2) {
      int move = rand()%4;
      if (move == 0) { 
        x++;
      } 
      else if (move == 1) { 
        x--;
      } 
      else if (move == 2) { 
        y++;
      } 
      else if (move == 3) { 
        y--;
      }
      if (col[y*WIDTH+x-1]==coll || col[y*WIDTH+x+1]==coll || col[(y+1)*WIDTH+x]==coll || col[(y-1)*WIDTH+x]==coll) {
        col[y*WIDTH+x] = coll;
        break;
      }
    }
  }

  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
