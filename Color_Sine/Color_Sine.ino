// Color Sine demo //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

uint16_t RGB565(uint8_t R, uint8_t G, uint8_t B){ return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );}

  uint16_t col[SCR];
  int t;

void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);

}

void loop(){

  t = millis() / 100;
       
  for(int j=0;j<HEIGHT;++j){

    for(int i=0;i<WIDTH;++i){
        
      uint8_t coll = 255.0f * (((sinf(j*sinf(t/40.0f)*M_PI*5.0f/HEIGHT)+1.0f)/4.0f) + ((sinf(i*sinf(t/77.0f)*M_PI*7.5f/WIDTH)+1.0f)/4.0f));

      col[i+j*WIDTH] = RGB565(coll, coll, coll);
         
    }
  }
   
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
