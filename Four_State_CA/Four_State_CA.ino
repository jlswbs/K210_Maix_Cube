// Four state 1D cellular automata //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

  uint16_t col[SCR];
  uint16_t coll0, coll1, coll2, coll3;
  uint8_t parent[WIDTH];
  uint8_t child[WIDTH];
  int count, i, j;

void rndrule(){

  parent[(WIDTH/2)-1] = 1 + rand()%3;//1
  parent[(WIDTH/2)] = 1 + rand()%3;//3
  parent[(WIDTH/2)+1] = 1 + rand()%3;//2
  coll0 = rand();
  coll1 = rand();
  coll2 = rand();
  coll3 = rand();
 
}


void setup()
{
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);

  srand(read_cycle());

  rndrule();

}

void loop(){

  for(j = 0; j < HEIGHT; j++) {
 
    for(i = 0; i < WIDTH; i++) {

      if(i == 0) count = parent[WIDTH-1] + parent[0] + parent[1];
      else if(i == WIDTH-1) count = parent[WIDTH-2] + parent[WIDTH-1] + parent[0];
      else count = parent[i-1] + parent[i] + parent[i+1];        
            
      if(count == 0 || count == 5 || count == 6) child[i] = 0;
      if(count == 3 || count == 4) child[i] = 1;
      if(count == 1 || count == 2) child[i] = 2;
      if(count == 7 || count == 5 || count == 1) child[i] = 3;
                
      if(child[i] == 0) col[i+j*WIDTH] = coll0;
      if(child[i] == 1) col[i+j*WIDTH] = coll1;
      if(child[i] == 2) col[i+j*WIDTH] = coll2;
      if(child[i] == 3) col[i+j*WIDTH] = coll3;
     
    }

    memcpy(parent, child, WIDTH);
    
    lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);                                                          
  
  }

}
