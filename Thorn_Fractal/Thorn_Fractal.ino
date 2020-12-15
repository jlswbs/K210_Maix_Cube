// Thorn fractal //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

uint16_t col[SCR];

uint16_t RGB565(uint8_t R, uint8_t G, uint8_t B){ return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );}

  #define ITER 250

  float xmin = -PI * 0.5f;
  float xmax = +PI * 0.5f;
  float ymin = -PI;
  float ymax = +PI;
  float ir,ii,zi,zr;
  float escape = 10000.0f;
  float ci,cr,t;
  int k;

  uint16_t coll[ITER];
  

void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);
  
  srand(read_cycle());
  
  float deltax = 1.0f;
  
  for (int i=0; i<ITER; i++){
    
    coll[i] = RGB565(128.0f+127.0f*cosf(deltax*0.022f),128.0f-127.0f*sinf(55.0f+deltax*0.049f),128.0f-127.0f*sinf(155.0f+deltax*0.075f));
    deltax *= 0.996f;
    deltax += 2.0f;
  
  }

}

void loop(){

  t = t + 0.1f;
    
  for (int i=0;i<WIDTH;i++) {
    
    zr = xmin + i * (xmax - xmin) / WIDTH;
  
      for (int j=0;j<HEIGHT;j++) {
        
        zi = ymin + j * (ymax - ymin) / HEIGHT;
        ir = zr;
        ii = zi;
        
        for (k=0;k<ITER;k++) {
          
          float a = ir;
          float b = ii;
          ir = a / cosf(b + t);
          ii = b / sinf(a + t);
          if (ir*ir + ii*ii > escape) break;

        }
  
        col[i+j*WIDTH] = coll[k];
        
      }
    }

  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
