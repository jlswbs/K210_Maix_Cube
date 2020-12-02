// Langtons Ant Multi //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

  uint16_t col[SCR];

  #define NUMANTS 5
  #define ITER 25
  
  uint16_t coll[NUMANTS];
  
  int x[NUMANTS];
  int y[NUMANTS];
  int antsdir[NUMANTS];


void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);

  srand(read_cycle());

  for(int i = 0; i < NUMANTS; i++){
  
    x[i] = rand()%WIDTH;
    y[i] = rand()%HEIGHT;
    antsdir[i] = rand()%4;
    coll[i] = rand();
    
  }
}

void loop(){
  
  for(int k = 0; k < ITER; k++){
  
    for(int i = 0; i < NUMANTS; i++){
    
      if (col[x[i]+WIDTH*y[i]] > COLOR_BLACK){ antsdir[i] = antsdir[i] - 1; col[x[i]+WIDTH*y[i]] = COLOR_BLACK; }
      else { antsdir[i] = antsdir[i] + 1; col[x[i]+WIDTH*y[i]] = coll[i]; }

      if (antsdir[i] > 3) antsdir[i] = 0;   
      if (antsdir[i] < 0) antsdir[i] = 3;   
    
      if (antsdir[i] == 0) x[i] = x[i] - 1;
      if (antsdir[i] == 1) y[i] = y[i] + 1;
      if (antsdir[i] == 2) x[i] = x[i] + 1;
      if (antsdir[i] == 3) y[i] = y[i] - 1;
    
      if (x[i] > WIDTH-1) x[i] = 0;
      if (x[i] < 0) x[i] = WIDTH-1;
      if (y[i] > HEIGHT-1) y[i] = 0;
      if (y[i] < 0) y[i] = HEIGHT-1;
    
    }
  }
  
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);
  
}
