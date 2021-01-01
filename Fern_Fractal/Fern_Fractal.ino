// Fern fractal //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

  uint16_t col[SCR];
  
  uint16_t xx, yy;
  
  int iterations = 100;

  float e1 = 0.5f;
  float e2 = 0.57f;
  float e3 = 0.408f;
  float e4 = 0.1075f;
  float f1 = 0.0f;
  float f2 = -0.036f;
  float f3 = 0.0893f;
  float f4 = 0.27f;
  float x = e1;
  float y = 0.0f;
  float nx,ny;
  

void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);

  srand(read_cycle());
     
}

void loop(){

  for (int i = 0; i < iterations; i++) {

    int z = (rand() & 0x7FFF);
    
    if (z > 164) goto _100;
      nx = (0.0f * x +  0.0f * y    + e1);
      ny = (0.0f * x +  0.27f * y + f1);
      goto _400;
    
    _100: if(z > 5570) goto _200;
      nx = (-0.139f * x +  0.263f * y + e2);
      ny = (0.246f * x  + 0.224f  * y + f2);
      goto _400;    
    
    _200: if(z > 9830) goto _300;
      nx = (0.17f * x -  0.215f * y + e3);
      ny = (0.222f * x + 0.176f * y + f3);
      goto _400;
          
    _300: 
      nx = (0.781f * x +  0.034f * y + e4);
      ny = (-0.032f * x + 0.739f * y + f4);
    
    _400:
      xx = (WIDTH * ny);
      yy = -(HEIGHT/1.8f) + ((2*HEIGHT) * nx);  
      if(xx > 0 && xx < WIDTH && yy > 0 && yy < HEIGHT) col[xx+yy*WIDTH] = COLOR_WHITE;
      x = nx;
      y = ny; 

  }
      
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
