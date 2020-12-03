// Gray-Scott Reaction-Diffusion //

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
  int i, j;
  
  float a[WIDTH][HEIGHT];
  float aNext[WIDTH][HEIGHT];
  float b[WIDTH][HEIGHT];
  float bNext[WIDTH][HEIGHT];
  float temp[WIDTH][HEIGHT];
  float deltaT = 5.0f; // 1.2 min - 5 max
  float reactionRate = 0.49f; // 0.62 - 0.49
  float aRate = 0.0385f; // 0.04 - 0.038
  float bRate = 0.008f; // 0.008 - 0.01
  float k = 0.031000046f; //.06; // 0.07 - 0.06
  float F = 0.0069999984f; //.015; // 0.04 - 0.015

  int iNext[WIDTH];
  int jNext[HEIGHT];
  int iPrev[WIDTH];
  int jPrev[HEIGHT];

    
void rndrule(){

  aRate = randomf(0.029f,0.049f);
  bRate = randomf(0.007f,0.015f);

  for(int i=0;i<WIDTH;++i)
  {
    iNext[i] = (i+1)%WIDTH;
    iPrev[i] = (i-1+WIDTH)%WIDTH;
  }
  
  for(int j=0;j<HEIGHT;++j)
  {
    jNext[j] = (j+1)%HEIGHT;
    jPrev[j] = (j-1+HEIGHT)%HEIGHT;
  }
  
   for(int i=(WIDTH-4)/2;i<(WIDTH+4)/2;++i) 
  {
    for(int j=(HEIGHT-4)/2;j<(HEIGHT+4)/2;++j) 
    {
      a[i][j] = 0.5f + randomf(-0.01f,0.01f);
      b[i][j] = 0.25f + randomf(-0.01f,0.01f);
    }
  }

}


void diffusion(){
  
  for(int i=0;i<WIDTH;++i) 
  {
    for(int j=0;j<HEIGHT;++j) 
    {
      aNext[i][j] = a[i][j]+aRate*deltaT*
        (a[iNext[i]][j]+a[iPrev[i]][j]
        +a[i][jNext[j]]+a[i][jPrev[j]]
        -4*a[i][j]);
      
      bNext[i][j] = b[i][j]+bRate*deltaT*
        (b[iNext[i]][j]+b[iPrev[i]][j]
        +b[i][jNext[j]]+b[i][jPrev[j]]
        -4*b[i][j]);
    }
  }
  
  for(int i=0;i<WIDTH;++i){  
    for(int j=0;j<HEIGHT;++j){
      
      temp[i][j] = a[i][j];
      a[i][j] = aNext[i][j];
      aNext[i][j] = temp[i][j];
      temp[i][j] = b[i][j];
      b[i][j] = bNext[i][j];
      bNext[i][j] = temp[i][j];
    }
  }
}
  
void reaction(){
  float valA, valB;

  for(int i=0;i<WIDTH;++i) 
  {
    for(int j=0;j<HEIGHT;++j) 
    {
      valA = deltaT*reactionA(a[i][j],b[i][j]);
      valB = deltaT*reactionB(a[i][j],b[i][j]);
      a[i][j] += valA;
      b[i][j] += valB;
    }
  }
}
  
float reactionA(float aVal, float bVal){ return reactionRate*(-aVal*bVal*bVal+F*(1.0f-aVal)); }
  
float reactionB(float aVal, float bVal){ return reactionRate*(aVal*bVal*bVal-(F+k)*bVal); }


void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);

  srand(read_cycle());

  rndrule();

}

void loop(){

  diffusion();
  reaction();

  for(int j=0;j<HEIGHT;++j){
    for(int i=0;i<WIDTH;++i){
      
      uint16_t coll = 64.0f * a[i][j];
      col[i+j*WIDTH] = gray2rgb565[coll%64];
         
    }
  }
   
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
