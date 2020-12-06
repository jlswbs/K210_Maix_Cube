// Mandelbulber Fractal //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

uint16_t RGB565(byte R, byte G, byte B){ return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );}
float randomf(float minf, float maxf) {return minf + (rand()%(1UL << 31))*(maxf - minf) / (1UL << 31);}
  
  uint16_t col[SCR];
  int maxIterations = 2;
  float power = 5.0f;
  float xmin = -1.0f;
  float ymin = -1.0f;
  float zmin = -1.0f;
  float wh = 2.0f;
  float addx = wh/WIDTH;
  float addy = wh/HEIGHT;
  float x = xmin, y;

void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);

  srand(read_cycle());

  power = randomf(1.5f, 8.0f);
  
}

void loop(){

  for (int i = 0; i < WIDTH; i++) {

    y = ymin;

    for (int j = 0; j < HEIGHT; j++) {
      
      float z = zmin;
      
      for (int k = 255; k > 0 && z < -1.0f*zmin; k--) {
        
        float dr = 1;
        float nx = x;
        float ny = y;
        float nz = z;
        float rad = sqrtf(x*x+y*y+z*z);
        int n = 0;
        
        while (n < maxIterations) {
          
          float powRad = powf(rad, power);
          float theta = atan2f(sqrtf(nx*nx+ny*ny), nz)*power;
          float phi = atan2f(ny, nx)*power;
          nx = sinf(theta)*cosf(phi)*powRad+x;
          ny = sinf(theta)*sinf(phi)*powRad+y;
          nz = cosf(theta)*powRad+z;
          dr = dr*power*powf(rad, power - 1.0f);
          rad = sqrtf(nx*nx+ny*ny+nz*nz);
          if (rad > 4.0f) break;
          n++;            
        }
        
        if (0.5f*logf(rad)*rad/dr < 0.00001f) {        
          col[i+j*WIDTH] = RGB565(k,k,k);
          break;       
        } else {
          k--;
          z += 0.5f*logf(rad)*rad/dr;  
        }   
      }
      
      y += addy;
    }
    
    x += addx;
    lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

  }
}
