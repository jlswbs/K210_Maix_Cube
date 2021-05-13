// Tetration fractal //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

uint16_t col[SCR];

float mapf(float val, float in_min, float in_max, float out_min, float out_max) {return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;}

#define ITER 20

float cpowera(float a, float b, float c, float d) {

  float r = sqrtf(a*a + b*b);
  float t = atan2f(b, a);

  return (powf(r, c)*powf(2.718281828459045f, -d*t)*cosf(d*logf(r) + c*t));
  
}

  
float cpowerb(float a, float b, float c, float d) {

  float r = sqrtf(a*a + b*b);
  float t = atan2f(b, a);
  
  return (powf(r, c)*powf(2.718281828459045f, -d*t)*sinf(d*logf(r) + c*t)); 
  
}

int tetrate(float a, float b) {

  int i = 0;
  
  float tm[2] = {a, b};
  
  while (i < ITER && sqrtf(tm[0]*tm[0]+tm[1]*tm[1]) < 4) {
    
    tm[0] = cpowera(a, b, tm[0], tm[1]);
    tm[1] = cpowerb(a, b, tm[0], tm[1]);
    
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
     
      uint16_t coll = tetrate(mapf(x, 0, WIDTH, -4.0f, 4.0f), mapf(y, 0, HEIGHT, -4.0f, 4.0f));   
      col[x+y*WIDTH] = coll;
      
    }
  }
      
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
