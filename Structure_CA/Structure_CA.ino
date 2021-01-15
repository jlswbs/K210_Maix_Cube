// Structure recursion cellular automata //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

  uint16_t col[SCR];
  
  uint8_t state[WIDTH];
  uint8_t k, x, y;
  

void rndrule(){

  for (x=0; x<WIDTH; x++) state[x] = rand()%16;
  k = rand()%16;

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
  
    for (x = 0; x < WIDTH; x++) {
      
      k = k ^ state[x] ^ state[k];
      state[x] = k;
      col[x+WIDTH*y] = (COLOR_WHITE>>k) ^ k;
      
    }
    
  }

  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
