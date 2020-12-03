// Chemical Diffusion-Reaction //

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
  
  uint16_t col[SCR];
  
  float u[WIDTH+2][HEIGHT+2];
  float v[WIDTH+2][HEIGHT+2];
  float u1[WIDTH+2][HEIGHT+2];
  float v1[WIDTH+2][HEIGHT+2];

  float dt = 1.2, h = 0.1, h2 = h*h;
  float a = 0.024, b = 0.078;
  float cu = 0.002, cv = 0.001;

    
void rndrule() {

  a = randomf(0.021f, 0.027f);
  b = randomf(0.074f, 0.081f);

  for (int i = 1; i <= WIDTH-1; i++){
    for (int j = 1; j <= HEIGHT-1; j++){
        u[i][j] = 0.5f + randomf(-0.5f, 0.5f);
        v[i][j] = 0.125f + randomf(-0.25f, 0.25f);
    }
  }
}

void update(){
  for (int i = 1; i <= WIDTH; i++) {
    for (int j = 1; j <= HEIGHT; j++) {
      float Du = (u[i+1][j] + u[i][j+1] + u[i-1][j] + u[i][j-1] - 4 * u[i][j]) / h2;
      float Dv = (v[i+1][j] + v[i][j+1] + v[i-1][j] + v[i][j-1] - 4 * v[i][j]) / h2;
      float f = - u[i][j] * sq(v[i][j]) + a * (1 - u[i][j]);
      float g = u[i][j] * sq(v[i][j]) - b * v[i][j];
      u1[i][j] = u[i][j] + (cu * Du + f) * dt;
      v1[i][j] = v[i][j] + (cv * Dv + g) * dt;
    }
  }
  for (int i = 1; i <= WIDTH; i++) {
    for (int j = 1; j <= HEIGHT; j++) {
      u[i][j] = u1[i][j];
      v[i][j] = v1[i][j];
    }
  }
}

void boundary(){
  for (int i = 1; i <= WIDTH; i++){
    u[i][0] = u[i][HEIGHT];
    u[i][HEIGHT+1] = u[i][1];
    u[0][i] = u[WIDTH][i];
    u[WIDTH+1][i] = u[1][i];
    v[i][0] = v[i][HEIGHT];
    v[i][HEIGHT+1] = v[i][1];
    v[0][i] = v[WIDTH][i];
    v[WIDTH+1][i] = v[1][i];
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

void loop(){       

  for (int y = 1; y <= HEIGHT; y++){
    for (int x = 1; x <= WIDTH; x++){
      uint16_t coll = 64.0f * u[x][y];
      col[x+y*WIDTH] = gray2rgb565[coll%64];
    }
  }
  
  boundary();
  update();

  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
