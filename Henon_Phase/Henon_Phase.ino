// Quadratic Henon attractor //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

uint16_t RGB565(uint8_t R, uint8_t G, uint8_t B){ return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );}

  uint16_t col[SCR];

  static uint16_t coll[256];
  
  float zoom = 1.0f;
  float t;

void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);
  
  srand(read_cycle());

  float deltax = 1.0f;
  
  for (int i=0; i<256; i++){
    
    coll[i] = RGB565(128.0f+127.0f*cosf(deltax*0.022f),128.0f-127.0f*sinf(55.0f+deltax*0.049f),128.0f-127.0f*sinf(155.0f+deltax*0.075f));
    deltax *= 0.996f;
    deltax += 2.0f;
  
  }

}

void loop(){

  memset(col,0,2*SCR); 

  t = t + 0.05f;

  float y = 0.0f;
  float sn = sinf(t);
  float cs = cosf(t);
  float xi = -1.2f;
  float xf = 1.2f;
  float yi= -1.2f;
  float yf = 1.2f;
  float sx = (xf-xi) / (WIDTH * zoom);
  float sy = (yf-yi) / (HEIGHT * zoom);
  uint8_t cl = 0;
  
  for(float xc = xi; xc < xf;  xc = xc + sx){

    float y = xc;
    float x = xc;
    int i = 1;

    while(i <= 1000){
          
      if (x > 1.0E+15) break;

      float yx = y - x * x;
      float xx = x * cs - yx * sn;
      y = x * sn + yx * cs;
      x = xx;
      int px = int((WIDTH/2) + x / sx);
      int py = int((HEIGHT/2) - y / sy);
      
      if (px > 0 && px <= WIDTH && py > 0 && py <= HEIGHT) col[px+py*WIDTH] = coll[cl];

      i = i + 1;
      
  }
    
    cl = cl + 1;
  
  }
 
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);
  
}
