// Voronoi distribution (cell noise) //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

static uint16_t gray2rgb565[64]={
  0x0000, 0x2000, 0x4108, 0x6108, 0x8210, 0xa210, 0xc318, 0xe318, 
  0x0421, 0x2421, 0x4529, 0x6529, 0x8631, 0xa631, 0xc739, 0xe739, 
  0x0842, 0x2842, 0x494a, 0x694a, 0x8a52, 0xaa52, 0xcb5a, 0xeb5a, 
  0x0c63, 0x2c63, 0x4d6b, 0x6d6b, 0x8e73, 0xae73, 0xcf7b, 0xef7b, 
  0x1084, 0x3084, 0x518c, 0x718c, 0x9294, 0xb294, 0xd39c, 0xf39c, 
  0x14a5, 0x34a5, 0x55ad, 0x75ad, 0x96b5, 0xb6b5, 0xd7bd, 0xf7bd, 
  0x18c6, 0x38c6, 0x59ce, 0x79ce, 0x9ad6, 0xbad6, 0xdbde, 0xfbde, 
  0x1ce7, 0x3ce7, 0x5def, 0x7def, 0x9ef7, 0xbef7, 0xdfff, 0xffff,
};

#define PARTICLES 16

  uint16_t col[SCR];

  float mindist = 0;
  int x[PARTICLES];
  int y[PARTICLES];
  int dx[PARTICLES];
  int dy[PARTICLES];

float distance(int x1, int y1, int x2, int y2) { return sqrtf(powf(x2 - x1, 2.0f) + powf(y2 - y1, 2.0f) * 1.0f); }

void rndrule(){
  
  for (int i=0; i<PARTICLES; i++) {
    x[i] = rand()%WIDTH;
    y[i] = rand()%HEIGHT;
    dx[i] = rand()%15;
    dy[i] = rand()%15;
  }

}
  
void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);

  srand(read_cycle());

  rndrule();

}

void loop(){

  for (int j=0; j<HEIGHT; j++) {
    
    for (int i=0; i<WIDTH; i++) {
      
      mindist = WIDTH;
      
      for (int p=0; p<PARTICLES; p++) {
        if (distance(x[p], y[p], i, j) < mindist) mindist = distance(x[p], y[p], i, j);
        if (distance(x[p]+WIDTH, y[p], i, j) < mindist) mindist = distance(x[p]+WIDTH, y[p], i, j);
        if (distance(x[p]-WIDTH, y[p], i, j) < mindist) mindist = distance(x[p]-WIDTH, y[p], i, j);
      }

      uint8_t coll = 2.0f * mindist;
      col[i+j*WIDTH] = gray2rgb565[(uint8_t)coll>>2];

    }
  }
  
  for (int p=0; p<PARTICLES; p++) {
    x[p] += dx[p];
    y[p] += dy[p];
    x[p] = x[p] % WIDTH;
    y[p] = y[p] % HEIGHT;
  }  
 
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
