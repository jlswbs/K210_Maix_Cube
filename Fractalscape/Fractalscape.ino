// Fractalscape patterns //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

uint16_t RGB565(uint8_t R, uint8_t G, uint8_t B){ return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );}

  uint16_t col[SCR];

  float ibs[HEIGHT];
  int iterations = 77;

  float sizeX = 1.0f / 3000.0f;
  float sizeY = sizeX * HEIGHT / WIDTH;
  float offsetX = 0.55f;
  float offsetY = -1.981f;

  int pixelStep = 8;
  float fractalStep = sizeX * pixelStep / WIDTH;

  
void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);
  
  srand(read_cycle());

  for (int i = 0; i < HEIGHT; i++) ibs[i] = offsetY + i * sizeY / HEIGHT;
  
}

void loop(){
  
  for (int x = pixelStep; x < WIDTH; x++) {
    int xBack = x - pixelStep;    
    for (int y = 0; y < HEIGHT; y++) {
      col[xBack+y*WIDTH] = col[x+y*WIDTH];
    }
  }
  
  offsetX += fractalStep;
  
  fractalize(WIDTH-pixelStep, WIDTH, 0, HEIGHT);

  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}

void fractalize(int x1, int x2, int y1, int y2) {
  
  for (int x = x1; x < x2; x++) {
    float ia = offsetX + x * sizeX / WIDTH;
    
    for (int y = y1; y < y2; y++) {
      float a = ia;
      float b = ibs[y];
      
      for (int i = 0; i < iterations; i++) {
        float ta = (a + ibs[y]) * b;
        b = (b + ia) * a;
        a = ta;
      }
      uint8_t coll1 = 64.0f * (a + 2.0f);
      uint8_t coll2 = 64.0f * (b + 2.0f);
      col[x+y*WIDTH] = RGB565(coll1, coll2, coll1);
    }
  }
}
