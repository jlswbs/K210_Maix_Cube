// Turmites cellular automata //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

  uint16_t col[SCR];

  int posx,posy;
  int oldposx, oldposy;
  int state;
  int dir;
  int world[WIDTH][HEIGHT];
  int moves;
  int filled;
  int last_filled;
  int current_col;
  int next_col[4][4];
  int next_state[4][4];
  int directions[4][4];


void rndrule(){

  state = rand()%4;
  filled = 0;
  moves = 0;
  dir = 0;
  posx = WIDTH/2;
  posy = HEIGHT/2;
  
  for(int j=0; j<4; j++){   
    for(int i=0; i<4; i++){         
      next_col[i][j] = rand()%4;
      next_state[i][j] = rand()%4;
      directions[i][j] = rand()%8;
    }   
  }

  for (int j=0; j < HEIGHT; j++){ for (int i = 0; i < WIDTH; i++) world[i][j]=0; } 

  world[posx][posy] = rand()%4;

}

void move_turmite(){
  
  moves ++;
  int cols = world[posx][posy];
  
  if((cols > 0) && (next_col[cols][state] == 0)) filled--;
  if((cols == 0) && (next_col[cols][state] > 0)) filled++;
  
  oldposx = posx;
  oldposy = posy;
  current_col = next_col[cols][state];
  world[posx][posy] = next_col[cols][state];
  state = next_state[cols][state];    

  dir = (dir + directions[cols][state]) % 8;

  switch(dir){
    case 0: posy--; break;
    case 1: posy--; posx++; break;
    case 2: posx++; break;
    case 3: posx++; posy++; break;
    case 4: posy++; break;
    case 5: posy++; posx--; break;
    case 6: posx--; break;
    case 7: posx--; posy--; break;
  }

  if(posy < 0) posy = HEIGHT-1;
  if(posy >= HEIGHT) posy = 0;
  if(posx < 0) posx = WIDTH-1;
  if(posx >= WIDTH) posx=0;
  
}

void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);

  srand(read_cycle());

  rndrule();
  
}

void loop(){

  for(int i=0; i<100; i++){
    
    move_turmite();
    
    switch(current_col){
      case 0: col[oldposx + oldposy * WIDTH] = COLOR_RED; break;
      case 1: col[oldposx + oldposy * WIDTH] = COLOR_GREEN; break;
      case 2: col[oldposx + oldposy * WIDTH] = COLOR_BLUE; break;
      case 3: col[oldposx + oldposy * WIDTH] = COLOR_WHITE; break;
    }
    
  }

  if((moves>1000) && (filled < 50)) rndrule();
  
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);
  
}
