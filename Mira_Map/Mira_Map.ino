// Mira chaotic map //

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
  float x = 1.0f;
  float y = 0.0f;
  float f = 0.0f;
  float a = 0.2f;


void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);
  
  srand(read_cycle());

  a = randomf(0.099f, 0.499f);

}

void loop(){

  uint16_t coll = rand();

  for (int i = 0; i < iterations; i++) {

    float nx = x;
    float ny = y;
        
    x = ny + f;
    f = a * x + ((2.0f*(1.0f-a)*powf(x,2.0f)) / (1.0f+powf(x,2.0f)));
    y = -nx + f;

    xx = -10 + (WIDTH/2) + 8.0f * x; 
    yy = (HEIGHT/2) + 8.0f * y;
    
    if(xx > 0 && xx < WIDTH && yy > 0 && yy < HEIGHT) col[xx+yy*WIDTH] = coll;

  } 
              
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
