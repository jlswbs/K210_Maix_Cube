// Turing patterns //

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

  #define SCL 4
  int lim = 128;
  int dirs = 9;
  int patt = 0;
  int x, y, i, j, l;
  
  uint16_t col[SCR];
  float pat[SCR];
  float pnew[SCR];
  float pmedian[SCR][SCL];
  float prange[SCR][SCL];
  float pvar[SCR][SCL];
  

void setup()
{
    lcd.begin(15000000, COLOR_BLACK);
    lcd.setRotation(2);
    tft_write_command(INVERSION_DISPALY_ON);

    srand(read_cycle());

    patt = rand()%3;
    for(i=0; i<SCR; i++) pat[i] = randomf(0, 255.0f);

}

void loop()
{

  float R = randomf(0, TWO_PI);

  memcpy(pnew, pat, 4 * SCR);

  for(j=0; j<SCL; j++)

    for(i=0; i<SCR; i++) {

      pmedian[i][j] = 0;
      prange[i][j] = 0;
      pvar[i][j] = 0;

    }

    for(i=0; i<SCL; i++) {

      float d = (2<<i);

      for(j=0; j<dirs; j++) {

        float dir = j*TWO_PI/dirs + R;
        int dx = (d * cos(dir));
        int dy = (d * sin(dir));
        
        for(l=0; l<SCR; l++) {
    
          int x1 = l%WIDTH + dx, y1 = l/WIDTH + dy;
          if(x1<0) x1 = WIDTH-1-(-x1-1); if(x1>=WIDTH) x1 = x1%WIDTH;
          if(y1<0) y1 = HEIGHT-1-(-y1-1); if(y1>=HEIGHT) y1 = y1%HEIGHT;
          pmedian[l][i] += pat[x1+y1*WIDTH] / dirs;

        }
      }

      for(j=0; j<dirs; j++) {

        float dir = j*TWO_PI/dirs + R;
        int dx = (d * cos(dir));
        int dy = (d * sin(dir));
    
        for(l=0; l<SCR; l++) {

          int x1 = l%WIDTH + dx, y1 = l/WIDTH + dy;

          if(x1<0) x1 = WIDTH-1-(-x1-1); if(x1>=WIDTH) x1 = x1%WIDTH;
          if(y1<0) y1 = HEIGHT-1-(-y1-1); if(y1>=HEIGHT) y1 = y1%HEIGHT;

          pvar[l][i] += fabs(pat[x1+y1*WIDTH] - pmedian[l][i]) / dirs;
          prange[l][i] += pat[x1+y1*WIDTH] > (lim + i*10) ? +1.0f : -1.0f;

        }
      }
    }

    for(l=0; l<SCR; l++) {

      int imin = 0, imax = SCL;
      float vmin = MAXFLOAT;
      float vmax = -MAXFLOAT;

      for(i=0; i<SCL; i+=1) {

        if (pvar[l][i] <= vmin) { vmin = pvar[l][i]; imin = i; }
        if (pvar[l][i] >= vmax) { vmax = pvar[l][i]; imax = i; }
      
      }

      switch(patt){
        case 0: for(i=0; i<=imin; i++)    pnew[l] += prange[l][i]; break;
        case 1: for(i=imin; i<=imax; i++) pnew[l] += prange[l][i]; break;
        case 2: for(i=imin; i<=imax; i++) pnew[l] += prange[l][i] + pvar[l][i] / 2.0f; break;
      }

    }

    float vmin = MAXFLOAT;
    float vmax = -MAXFLOAT;

    for(i=0; i<SCR; i++) {

      vmin = min(vmin, pnew[i]);
      vmax = max(vmax, pnew[i]);
        
    }

    float dv = vmax - vmin;
        

    for(i=0; i<SCR; i++){
          
      pat[i] = (pnew[i] - vmin) * 255 / dv;
      col[i] = gray2rgb565[(uint16_t)pat[i]>>2];
          
    }

    lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
