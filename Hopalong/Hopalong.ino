// Hopalong orbit fractal //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

uint16_t col[SCR];
    
float randomf(float minf, float maxf) {return minf + (rand()%(1UL << 31))*(maxf - minf) / (1UL << 31);}

  float a, b, c, x, y, t;

void rndrule(){

  x = 0.1f;
  y = 0.1f;
  
  float logpmax = logf(WIDTH/4);
  
  a = expf(randomf(0.0f, 1.0f)*logpmax);
  b = expf(randomf(0.0f, 1.0f)*logpmax);
  c = expf(randomf(0.0f, 1.0f)*logpmax);

}

void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);
  
  srand(read_cycle());

  rndrule();

}

void loop(){

  uint16_t coll = rand();

  for (int i=0;i<10000;i++) {

    float nx = x;
    float ny = y;

    t = sqrtf(fabs(b*nx-c));
    x = ny - ((nx<0) ? t : -t);
    y = a - nx;

    int ax = (WIDTH/2) + x;
    int ay = (HEIGHT/2) + y;
      
    if (ax>0 && ax<WIDTH && ay>0 && ay <HEIGHT) col[ax+ay*WIDTH] = coll;

  }
   
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);
  
}
