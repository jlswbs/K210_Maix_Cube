// Bars demo //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

uint16_t RGB565(uint8_t R, uint8_t G, uint8_t B){ return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );}

  uint16_t col[SCR];

void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);

}

void loop(){
  
  static float rv, gv, bv;
  
  rv += 0.03;
  gv += 0.01;
  bv += 0.02;
  
  uint8_t rp = 120 + 96 * sin(rv);
  uint8_t gp = 120 + 96 * sin(gv);
  uint8_t bp = 120 + 96 * sin(bv);
    
  for (int y=0;y<HEIGHT;y++){
    
    for (int x=0;x<WIDTH;x++) {

      uint8_t rc = 15 * max(16 - abs(y - rp), 0);
      uint8_t gc = 15 * max(16 - abs(y - gp), 0);
      uint8_t bc = 15 * max(16 - abs(y - bp), 0);
            
      col[x+y*WIDTH] = RGB565(rc, gc, bc);
        
    }

  }

  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);
 
}
