// Mandelbrot Fractal //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

uint16_t col[SCR];

void mandelbrot(float xMin, float xMax, float yMin, float yMax, int iterations){
  
  float x, y;
  float xStep, yStep; 
  float z, zi, newZ, newZI;

  xStep = (xMax - xMin) / WIDTH;
  yStep = (yMax - yMin) / HEIGHT;

  x = xMin;
  y = yMin;
  
  for (int j = 0; j < HEIGHT; j++){
    for (int i = 0; i < WIDTH; i++){
      
      z = 0;
      zi = 0;

      for (int k = 0; k < iterations; k++){
        
        newZ = (z * z) - (zi * zi) + x;
        newZI = 2 * z * zi + y;
        z = newZ;
        zi = newZI;
        
        if(((z * z) + (zi * zi)) > 4) col[i+j*WIDTH] = COLOR_WHITE / k;
      }

      x += xStep;
    }
    
    y += yStep;
    x = xMin;
  }
    
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);
}

void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);

  mandelbrot(-2.0f, 0.5f, -1.0f, 1.0f, 255);
  
}

void loop(){

}
