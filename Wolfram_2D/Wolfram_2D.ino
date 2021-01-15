// Wolfram 2D cellular automata //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

  uint16_t col[SCR];
  
  bool state[SCR];
  bool newst[SCR];

  uint16_t i, x, y;
  
  bool rules[10] = {0,0,1,1,1,1,0,0,0,0};


void rndrule(){

  state[(WIDTH/2)+(HEIGHT/2)*WIDTH] = 1;

  rules[0] = rand()%2;
  rules[1] = rand()%2;
  rules[2] = rand()%2;
  rules[3] = rand()%2;
  rules[4] = rand()%2;
  rules[5] = rand()%2;
  rules[6] = rand()%2;
  rules[7] = rand()%2;
  rules[8] = rand()%2;
  rules[9] = rand()%2;

}

uint8_t neighbors(uint16_t x, uint16_t y){
  
  uint8_t result = 0;

  if(y > 0 && state[x+(y-1)*WIDTH] == 1) result = result + 1;
  if(x > 0 && state[(x-1)+y*WIDTH] == 1) result = result + 1;
  if(x < WIDTH-1 && state[(x+1)+y*WIDTH] == 1) result = result + 1;
  if(y < HEIGHT-1 && state[x+(y+1)*WIDTH] == 1) result = result + 1;
  
  return result;
 
}


void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);
  
  srand(read_cycle());

  rndrule();

}


void loop(){

  for(y = 0; y < HEIGHT; y++){
    
    for(x = 0; x < WIDTH; x++){
           
      uint8_t totalNeighbors = neighbors(x,y);
            
      if(state[x+y*WIDTH] == 0 && totalNeighbors == 0) {newst[x+y*WIDTH] = rules[0]; col[x+y*WIDTH] = COLOR_BLACK;}
      else if(state[x+y*WIDTH] == 1 && totalNeighbors == 0) {newst[x+y*WIDTH] = rules[1]; col[x+y*WIDTH] = COLOR_RED;}
      else if(state[x+y*WIDTH] == 0 && totalNeighbors == 1) {newst[x+y*WIDTH] = rules[2]; col[x+y*WIDTH] = COLOR_GREEN;}
      else if(state[x+y*WIDTH] == 1 && totalNeighbors == 1) {newst[x+y*WIDTH] = rules[3]; col[x+y*WIDTH] = COLOR_BLUE;}
      else if(state[x+y*WIDTH] == 0 && totalNeighbors == 2) {newst[x+y*WIDTH] = rules[4]; col[x+y*WIDTH] = COLOR_YELLOW;}
      else if(state[x+y*WIDTH] == 1 && totalNeighbors == 2) {newst[x+y*WIDTH] = rules[5]; col[x+y*WIDTH] = COLOR_DARKGREY;}
      else if(state[x+y*WIDTH] == 0 && totalNeighbors == 3) {newst[x+y*WIDTH] = rules[6]; col[x+y*WIDTH] = COLOR_MAGENTA;}
      else if(state[x+y*WIDTH] == 1 && totalNeighbors == 3) {newst[x+y*WIDTH] = rules[7]; col[x+y*WIDTH] = COLOR_CYAN;}
      else if(state[x+y*WIDTH] == 0 && totalNeighbors == 4) {newst[x+y*WIDTH] = rules[8]; col[x+y*WIDTH] = COLOR_NAVY;}
      else if(state[x+y*WIDTH] == 1 && totalNeighbors == 4) {newst[x+y*WIDTH] = rules[9]; col[x+y*WIDTH] = COLOR_WHITE;}
      
    }
  }
 
  memcpy(state, newst, SCR);
      
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
