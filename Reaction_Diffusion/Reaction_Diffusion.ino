// Reaction-Diffusion //

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
  int i,j;

  float U[WIDTH][HEIGHT];
  float V[WIDTH][HEIGHT];
  float dU[WIDTH][HEIGHT];
  float dV[WIDTH][HEIGHT];
  float offsetW[WIDTH][2];
  float offsetH[HEIGHT][2];

  float diffU = 0.19f;
  float diffV = 0.09f;
  float paramF = 0.062f;
  float paramK = 0.062f;

    
void rndrule() {

  diffU = randomf(0.12f, 0.25f);
  diffV = randomf(0.081f, 0.099f);
    
  for (int y = 0; y < WIDTH; y++) {
    for (int x = 0; x < HEIGHT; x++){
      U[x][y] = 0.5f*(1.0f + randomf(-1.0f, 1.0f));
      V[x][y] = 0.25f*(1.0f + randomf(-1.0f, 1.0f));
    }
  }

  for (int i = 1; i < WIDTH-1; i++) {
    offsetW[i][0] = i-1;
    offsetW[i][1] = i+1;
  }
 
  offsetW[0][0] = WIDTH-1;
  offsetW[0][1] = 1;
  offsetW[WIDTH-1][0] = WIDTH-2;
  offsetW[WIDTH-1][1] = 0;

  for (int i = 1; i < HEIGHT-1; i++) {
    offsetH[i][0] = i-1;
    offsetH[i][1] = i+1;
  }
 
  offsetH[0][0] = HEIGHT-1;
  offsetH[0][1] = 1;   
  offsetH[HEIGHT-1][0] = HEIGHT-2;
  offsetH[HEIGHT-1][1] = 0;
  
}

void timestep(float F, float K, float diffU, float diffV) {
      
  for (int j = 0; j < HEIGHT; j++) {
    for (int i = 0; i < WIDTH; i++) {
           
      float u = U[i][j];
      float v = V[i][j];
           
      int left = offsetW[i][0];
      int right = offsetW[i][1];
      int up = offsetH[j][0];
      int down = offsetH[j][1];
           
      float uvv = u*v*v;    
        
      float lapU = (U[left][j] + U[right][j] + U[i][up] + U[i][down] - 4.0f * u);
      float lapV = (V[left][j] + V[right][j] + V[i][up] + V[i][down] - 4.0f * v);
           
      dU[i][j] = diffU * lapU - uvv + F * (1.0f - u);
      dV[i][j] = diffV * lapV + uvv - (K + F) * v;
    }
  }
       
  for (int j = 0; j < HEIGHT; j++){
    for (int i= 0; i < WIDTH; i++) {
      U[i][j] += dU[i][j];
      V[i][j] += dV[i][j];
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

void loop(){

  timestep(paramF, paramK, diffU, diffV);
    
  for (int j = 0; j < HEIGHT; j++) {
    for (int i = 0; i < WIDTH; i++) {
      uint16_t coll = 64.0f * (1.0f-U[i][j]);
      col[i+j*WIDTH] = gray2rgb565[coll%64];
    }
  }
   
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
