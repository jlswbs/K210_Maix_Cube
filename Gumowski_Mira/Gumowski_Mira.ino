// Gumowski-Mira Strange Attractor //

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
  
  int iterations = 10000;
  float a = 0.000001f;
  float b = 0.05f;
  float m = -0.031f;

  float nx = 0.5f;
  float ny = 0.5f;

float mira(float x){
  
  float xx = x * x;
  return (m * x) + ((2.0f * (1.0f - m) * xx) / (1.0f + xx));
  
}

void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);
  
  srand(read_cycle());

  m = randomf(-0.95f, -0.05f);

}

void loop(){

  uint16_t coll = rand();

  for (int i = 0; i < iterations; i++) {

    float x = nx;      
    float y = ny;
    
    nx = y + a * (1.0f - (b * (y * y))) * y + mira(x);
    ny = -x + mira(nx);

    xx = -4.0f + (WIDTH/2) + 3.0f * nx; 
    yy = (HEIGHT/2) + 3.0f * ny;
    
    if(xx > 0 && xx < WIDTH && yy > 0 && yy < HEIGHT) col[xx+yy*WIDTH] = coll;  

  }
              
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
