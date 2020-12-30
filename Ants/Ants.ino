// Ants traces //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

uint16_t RGB565(uint8_t R, uint8_t G, uint8_t B){ return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );}

  uint16_t col[SCR];

  #define PARTICLES 64
  int c = 0;

  float x[PARTICLES];
  float y[PARTICLES];
  float vx[PARTICLES];
  float vy[PARTICLES];


void setup(){

  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);
  
  srand(read_cycle());
  
  for (int c = 0; c < PARTICLES-1; c++) {
    x[c] = rand()%WIDTH;
    y[c] = rand()%HEIGHT;
  }
  
}

void loop(){

  if (c >= PARTICLES-1) c = 0;

  for (int a = 0; a < c; a++) {
    float ax = 0, ay = 0;
    
    for (int b = 0; b < c; b++) {
      if (a != b) {
       float dx = x[a] - x[b];
       float dy = y[a] - y[b];

        float d = sqrtf(dx*dx + dy*dy);
        if (d < 1.0f) d = 1.0f;

        float common = cosf(d) / d;

        ax += common * dx;
        ay += common * dy;
      }
    }

    vx[a] += ax * 0.01f;
    vy[a] += ay * 0.01f;
  }
  
  for (int i = 0; i < c; i++) {
    x[i] += vx[i];
    y[i] += vy[i];   
    uint8_t coll = i;
    if (x[i]>0 && x[i]<WIDTH && y[i]>0 && y[i]<HEIGHT) col[(int)(x[i])+(int)(y[i])*WIDTH] = RGB565(coll,coll,coll);
  }

  c++;
      
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
