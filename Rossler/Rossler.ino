// Rossler chaotic attractor //

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

uint16_t col[SCR];

#define ITER 16384

  float x = 0.1f;
  float y = 0.1f;
  float z = 0.1f;
  float dt = 0.003f;
  float a = 0.2f;
  float b = 0.2f;
  float c = 14.0f;

void setup(){

  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);
  
  srand(read_cycle());
  
}

void loop(){

  memset(col, 0, 2*SCR);

  for (int i=0; i<ITER; i++){

    float nx = x;
    float ny = y;
    float nz = z;
        
    x = nx + dt * (- ny - nz);
    y = ny + dt * (nx + a * ny);
    z = nz + dt * (b + nz * (nx - c)); 

    int ax = (WIDTH/2) + (4*x);
    int ay = (HEIGHT/2.3f) - (4*y);

    uint8_t coll = i<<4;
    
    if (ax>0 && ax<WIDTH && ay>0 && ay <HEIGHT) col[ax+ay*WIDTH] = gray2rgb565[(uint8_t)coll>>2];

  }

  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
