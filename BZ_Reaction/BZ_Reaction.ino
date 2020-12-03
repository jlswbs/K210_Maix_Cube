// Belousov-Zhabotinsky Reaction //

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

  float adjust = 1.2f;
  
  uint16_t col[SCR];
  
  float a [WIDTH][HEIGHT][2];
  float b [WIDTH][HEIGHT][2];
  float c [WIDTH][HEIGHT][2];

  int p = 0, q = 1;
  int x,y,i,j;

    
void rndrule() {

  adjust = randomf(0.75f, 1.35f);

  for (y = 0; y < HEIGHT; y++) {

    for (x = 0; x < WIDTH; x++) {

      a[x][y][p] = randomf(0.0f, 1.0f);
      b[x][y][p] = randomf(0.0f, 1.0f);
      c[x][y][p] = randomf(0.0f, 1.0f);
    }
  }
}

void setup()
{
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);

  srand(read_cycle());

  rndrule();

}

void loop()
{

  for (y = 0; y < HEIGHT; y++) {

    for (x = 0; x < WIDTH; x++) {

      float c_a = 0.0f;
      float c_b = 0.0f;
      float c_c = 0.0f;

      for (i = x - 1; i <= x+1; i++) {

        for (j = y - 1; j <= y+1; j++) {

          c_a += a[(i+WIDTH)%WIDTH][(j+HEIGHT)%HEIGHT][p];
          c_b += b[(i+WIDTH)%WIDTH][(j+HEIGHT)%HEIGHT][p];
          c_c += c[(i+WIDTH)%WIDTH][(j+HEIGHT)%HEIGHT][p];

        }
      }

      c_a /= 9.0f;
      c_b /= 9.0f;
      c_c /= 9.0f;

      a[x][y][q] = constrain(c_a + c_a * (adjust * c_b - c_c), 0.0f, 1.0f);
      b[x][y][q] = constrain(c_b + c_b * (c_c - adjust * c_a), 0.0f, 1.0f);
      c[x][y][q] = constrain(c_c + c_c * (c_a - c_b), 0.0f, 1.0f);
      
      uint16_t coll = 64.0f * a[x][y][q];
      col[x+y*WIDTH] = gray2rgb565[coll%64];         

    }
  }
  
  if (p == 0) { p = 1; q = 0; } else { p = 0; q = 1; }

  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
