// FitzHugh-Nagumo Reaction-Diffusion //

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

  void rndrule();
  void diffusionU();
  void diffusionV();
  void reaction();
  void setupF();
  void setupK();
  void setupDiffRates();

  uint16_t col[SCR];
  int i,j;

  float gridU[WIDTH][HEIGHT];
  float gridV[WIDTH][HEIGHT];
  float gridNext[WIDTH][HEIGHT];

  float reactionRate = 0.2f;
  
  float diffusionRateV = .01f; //the activator
  float diffusionRateU = .04f; // the inhabitor
  float diffusionRateUX = .04f;
  float diffusionRateUY = .03f;

  float diffRateUYarr[WIDTH][HEIGHT];
  float diffRateUXarr[WIDTH][HEIGHT];

  float deltaT = 4.0f; // time step

  float Farr[WIDTH][HEIGHT];
  float karr[WIDTH][HEIGHT];
  float temp[WIDTH][HEIGHT];

    
void rndrule() {

  diffusionRateV = randomf(0.01f, 0.05f);
  diffusionRateU = randomf(0.01f, 0.05f);
  diffusionRateUX = randomf(0.01f, 0.09f);
  diffusionRateUY = randomf(0.01f, 0.09f);

  i = 0;
  j = 0;

  for(i=0;i<WIDTH;++i) {
    for(j=0;j<HEIGHT;++j) {

      gridU[i][j] = 0.5f + randomf(-0.01f, 0.01f);
      gridV[i][j] = 0.25f + randomf(-0.01f, 0.01f);

    }
  }

  setupF();
  setupK();
  setupDiffRates();

}


void diffusionU() {

  for(i=0;i<WIDTH;++i) {

    for(j=0;j<HEIGHT;++j) {

      gridNext[i][j] = gridU[i][j]+deltaT*(diffRateUXarr[i][j]*(gridU[(i-1+WIDTH)%WIDTH][j]+gridU[(i+1)%WIDTH][j]-2*gridU[i][j])+diffRateUYarr[i][j]*(gridU[i][(j-1+HEIGHT)%HEIGHT]
        +gridU[i][(j+1)%HEIGHT]-2*gridU[i][j]));

  }
  }

  for(i=0;i<WIDTH;++i) {

    for(j=0;j<HEIGHT;++j) {

      temp[i][j] = gridU[i][j];
      gridU[i][j] = gridNext[i][j];
      gridNext[i][j] = temp[i][j];
    }
  }

}


void diffusionV() {

  for(i=0;i<WIDTH;++i) {

    for(j=0;j<HEIGHT;++j) {

      gridNext[i][j] = gridV[i][j]+diffusionRateV*deltaT*(gridV[(i-1+WIDTH)%WIDTH][j]+gridV[(i+1)%WIDTH][j]+gridV[i][(j-1+HEIGHT)%HEIGHT]+gridV[i][(j+1)%HEIGHT]-4*gridV[i][j]);
    }
  }


  for(i=0;i<WIDTH;++i) {

    for(j=0;j<HEIGHT;++j) {

      temp[i][j] = gridV[i][j];
      gridV[i][j] = gridNext[i][j];
      gridNext[i][j] = temp[i][j];

    }
  }
}


void reaction() {

  for(i=0;i<WIDTH;++i) {

    for(j=0;j<HEIGHT;++j) {

      gridU[i][j] = gridU[i][j] + deltaT*(reactionRate*(gridU[i][j]-gridU[i][j]*gridU[i][j]*gridU[i][j]-gridV[i][j]+karr[i][j]));
      gridV[i][j] = gridV[i][j] + deltaT*(reactionRate*Farr[i][j]*(gridU[i][j]-gridV[i][j]));

    }
  }
}


void setupF() {

  for(i=0;i<WIDTH;++i) {

    for(j=0;j<HEIGHT;++j) Farr[i][j] = 0.01f + i * 0.09f / WIDTH;

  }
}


void setupK() {

  for(i=0;i<WIDTH;++i) {

    for(j=0;j<HEIGHT;++j) karr[i][j] = 0.06f + j * 0.4f / HEIGHT;

  }
}


void setupDiffRates() {

  for(i=0;i<WIDTH;++i) {

    for(j=0;j<HEIGHT;++j) {

      diffRateUYarr[i][j] = 0.03f + j * 0.04f / HEIGHT;
      diffRateUXarr[i][j] = 0.03f + j * 0.02f / HEIGHT;

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

  diffusionU();
  diffusionV();
  reaction();

  for(i=0;i<WIDTH;++i) {

    for(j=0;j<HEIGHT;++j) {

      uint16_t coll = 64.0f * gridU[i][j];
      col[i+j*WIDTH] = gray2rgb565[coll%64];         
    }
  }
   
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
