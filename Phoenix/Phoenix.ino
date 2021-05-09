// Phoenix fractal //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

uint16_t col[SCR];

float mapf(float val, float in_min, float in_max, float out_min, float out_max) {return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;}

#define ITER 100

  float p = 0.5667f;
  float q = -0.5f;

float multa(float a, float b, float c, float d) { return (a*c - b*d); }
float multb(float a, float b, float c, float d) { return (a*d + b*c); }

int henon(float a, float b) {
  
  float pzr = 0;
  float pzi = 0;
  float zr = a;
  float zi = b;
  int i = 0;
  float ph1[2], ph2[2];
  
  while (i < ITER && sqrtf(zr*zr + zi*zi) < 4) {
    ph1[0] = multa(zr, zi, zr, zi);
    ph1[1] = multb(zr, zi, zr, zi);
    ph2[0] = multa(pzr, pzi, q, 0);
    ph2[1] = multb(pzr, pzi, q, 0);
    
    pzr = zr;
    pzi = zi;
    
    zr = ph1[0] + p + ph2[0];
    zi = ph1[1] + ph2[1];
    
    i++;
  }
  
  return map(i, 0, ITER, 65535, 0);

}

void setup(){

  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);
  
  srand(read_cycle());
  
}

void loop(){

  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
     
      uint16_t coll = henon(mapf(x, 0, WIDTH, -2.0f, 2.0f), mapf(y, 0, HEIGHT, -2.0f, 2.0f));   
      col[x+y*WIDTH] = coll;
      
    }
  }
      
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
