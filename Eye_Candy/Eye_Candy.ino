// Eye Candy patterns //

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

float randomf(float minf, float maxf) {return minf + (rand()%(1UL << 31))*(maxf - minf) / (1UL << 31);}

  uint16_t col[SCR];

  float p[WIDTH][HEIGHT];
  float v[WIDTH][HEIGHT];
  float a[WIDTH][HEIGHT];

  
void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);
  
  srand(read_cycle());

  for (int y = 1; y < HEIGHT-1; y=y+16) {
    for (int x = 1; x < WIDTH-1; x=x+16) {
      v[x][y] += ((2*PI) * randomf(-1.0f, 1.0f));
    }
  }
  
}

void loop(){
  
  for (int y = 1; y < HEIGHT-1; y++) {
    for (int x = 1; x < WIDTH-1; x++) {
      a[x][y] = (v[x-1][y] + v[x+1][y] + v[x][y-1] + v[x][y+1]) * 0.25f - v[x][y];
    }
  }
  
  for (int y = 1; y < HEIGHT-1; y++) {
    for (int x = 1; x < WIDTH-1; x++) {
      v[x][y] += a[x][y];
      p[x][y] += v[x][y];
      uint8_t coll = 63.0f * sinf(p[x][y]);
      col[x+y*WIDTH] = gray2rgb565[coll%64];
    }
  }

  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
