// Zaslavsky web map //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

float randomf(float minf, float maxf) {return minf + (rand()%(1UL << 31))*(maxf - minf) / (1UL << 31);}

  uint16_t col[SCR];
  uint16_t xx, yy;
  
  int iterations = 4096;

  float x = 0.1f;
  float y = 0.1f;
  float a = 0.19f;      
  float dt = 0.0f;


void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);
  
  srand(read_cycle());

  a = randomf(0.1999f, 0.9999f);

}

void loop(){

  uint16_t coll = rand();

  for (int i = 0; i < iterations; i++) {

    float nx = x;
    float ny = y;
        
    x = ny;
    y = -nx - a * sinf(2.0f * PI * ny - dt);

    dt = dt + 0.005f;

    xx = (WIDTH/2) + (x/3);
    yy = (HEIGHT/2) + (y/3);
    
    if(xx > 0 && xx < WIDTH && yy > 0 && yy < HEIGHT) col[xx+yy*WIDTH] = coll;  

  }
              
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
