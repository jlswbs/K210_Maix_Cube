// Color Candy patterns //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

uint16_t RGB565(uint8_t R, uint8_t G, uint8_t B){ return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );}

float randomf(float minf, float maxf) {return minf + (rand()%(1UL << 31))*(maxf - minf) / (1UL << 31);}

  static uint16_t coll[256];

  uint16_t col[SCR];

  float p[WIDTH][HEIGHT];
  float v[WIDTH][HEIGHT];
  float a[WIDTH][HEIGHT];

  
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

  for (int i = 0; i < 16; i++) v[16+rand()%(WIDTH-32)][16+rand()%(HEIGHT-32)] += (2*PI) * randomf(-1.0f, 1.0f);
  
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
      uint8_t cl = 128.0f + (127.0f * sinf(p[x][y]));
      col[x+y*WIDTH] = coll[cl];
    }
  }

  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
