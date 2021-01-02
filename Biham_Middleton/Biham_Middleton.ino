// Biham-Middleton-Levine Traffic Model //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

float randomf(float minf, float maxf) {return minf + (rand()%(1UL << 31))*(maxf - minf) / (1UL << 31);}

  uint16_t col[SCR];

  float density = 35;

  int rule[] = {0,0,0,0,2,2,1,1,1,2,2,2,0,2,2,1,1,1,0,0,0,0,2,2,1,1,1};
  int pixles[SCR];
  int slexip[SCR];

int color2state(int c){ return c == COLOR_RED ? 1 : (c == COLOR_GREEN ? 2 : 0); }
int state2color(int s){ return s == 1 ? COLOR_RED : (s == 2 ? COLOR_GREEN : COLOR_WHITE); }

void trafficSet(){

  memset(col, 0, sizeof(col));
  memset(pixles, 0, sizeof(pixles));
  
  for(int x=0; x<WIDTH; x++){
    for(int y=0; y<HEIGHT; y++){
      if((rand()%100)<density){
        if((rand()%100)<50) col[y*WIDTH+x] = COLOR_RED;
        else col[y*WIDTH+x] = COLOR_GREEN;
      }
    }
  }
  
  for(int i = 0; i<SCR; i++) pixles[i] = color2state(col[i]);

}

void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);
  
  srand(read_cycle());

  density = 1 + rand()%99;
  trafficSet();

}

void loop(){

  for(int x = 0; x<WIDTH; x++){
    for(int y = 0; y<HEIGHT; y++){
      slexip[y*WIDTH+x] = rule[pixles[y*WIDTH+(x+1)%WIDTH]+3*pixles[y*WIDTH+x]+9*pixles[y*WIDTH+(x+WIDTH-1)%WIDTH]];
    }
  }

  for(int x = 0; x<WIDTH; x++){
    for(int y = 0; y<HEIGHT; y++){
      pixles[y*WIDTH+x] = rule[slexip[x+((y+1)%HEIGHT)*WIDTH]+3*slexip[y*WIDTH+x]+9*slexip[x+((y+HEIGHT-1)%HEIGHT)*WIDTH]];
    }
  }
      
  for(int i = 0; i<SCR; i++) col[i] = state2color(pixles[i]);

  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
