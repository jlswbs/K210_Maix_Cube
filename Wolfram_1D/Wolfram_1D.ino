// Wolfram 1D cellular automata //

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

  uint8_t k, x, y;
  uint16_t coll;
  
  bool rules[8] = {0, 1, 1, 1, 1, 0, 0, 0};


void rndrule(){

  for (x=0; x<WIDTH; x++) state[x] = rand()%2;

  rules[0] = rand()%2;
  rules[1] = rand()%2;
  rules[2] = rand()%2;
  rules[3] = rand()%2;
  rules[4] = rand()%2;
  rules[5] = rand()%2;
  rules[6] = rand()%2;
  rules[7] = rand()%2;

}


void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);
  
  srand(read_cycle());

  rndrule();

}


void loop(){

  for (y = 0; y < HEIGHT; y++) {

    memset (newst, 0, sizeof(newst));

    for (x=0; x<WIDTH; x++) {     
      k = 4*state[(x-1+WIDTH)%WIDTH] + 2*state[x] + state[(x+1)%WIDTH];
      newst[x] = rules[k];
    }

    memcpy (state, newst, sizeof(state));
  
    for (x = 0; x < WIDTH; x++) {
      if (state[x] == 1) coll = COLOR_WHITE;       
      else coll = COLOR_BLACK;
      col[x+WIDTH*y] = coll;
    }

  }
      
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
