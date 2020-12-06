// Sand simulation //

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
  
  int M = 15;

  uint8_t C[WIDTH][HEIGHT]; 
  int X; int Y; int L; int R; int K;

void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);

  srand(read_cycle());

  for (X = 1 ; X < WIDTH-1; X++){
    for (Y = 1 ; Y < HEIGHT-1; Y++){
      if (Y < HEIGHT/4) C[X][Y] = 255*Y/(HEIGHT/4);
      if ((Y == HEIGHT/2 && (rand()%100) > 10) || X == 0 || X == WIDTH-1 || Y == 0 || Y == HEIGHT-1) C[X][Y] = 255;
    }
  }

}

void loop(){

  memset(col, 0, sizeof(col));
  
  for (Y = HEIGHT-4; Y > 0; Y--){
    if ((Y/2)*2 == Y){
      for (X = 0; X < WIDTH; X++){
        if (C[X][Y] == 0){
          R = C[X][Y-1];
          K = 0; 
          L = C[X-1][Y-1]; 
          MOVE();
        }
      }
    }
    if ((Y/2)*2 != Y){
      for (X = WIDTH; X > 0; X--){
        if (C[X][Y] == 0){
          L = C[X][Y-1];
          K = 1; 
          R = C[X+1][Y+1]; 
          MOVE() ;
        }
      }
    }        
  }  

  for (Y = 0 ; Y < HEIGHT; Y++){
    for (X = 0 ; X < WIDTH; X++){
      if (C[X][Y] > 0) col[X+Y*WIDTH] = gray2rgb565[(uint8_t)(C[X][Y])>>2];
    }
  }
  
  X = rand()%(WIDTH);
  Y = (HEIGHT/2-10)+rand()%(HEIGHT/2+10);
  if (C[X][Y] == 255) C[X][Y] = 0;
  
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);
  
}


void MOVE(){
  
    if ( ( L > R+M || R == 255 || R == 0) && L < 255){
      C[X][Y] = C[X+(K-1)][Y-1] ;
      C[X+(K-1)][Y-1] = 0 ;
    }
    if ( ( R > L+M || L == 255 || L == 0) && R < 255){
      C[X][Y] = C[X+K][Y-1];
      C[X+K][Y-1] = 0; 
    }
    if (abs(L-R) <= M && L < 255 && R < 255){
      if ((rand()%100) < 50+((L-R)*(50/M))){
        C[X][Y] = C[X+(K-1)][Y-1];
        C[X+(K-1)][Y-1] = 0;
      }else{
        C[X][Y] = C[X+K][Y-1];
        C[X+K][Y-1] = 0;
      }
    }
}
