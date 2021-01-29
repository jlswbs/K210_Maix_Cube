// Super Langtons Ant //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

  uint16_t col[SCR];

#define LENGHT 32

  uint16_t state[WIDTH][HEIGHT];
  uint16_t antX = WIDTH/2;
  uint16_t antY = HEIGHT/2;
  uint16_t direction;
  uint16_t stateCount;
  bool type[LENGHT];
  uint16_t stateCols[LENGHT];


void setup(){

  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);
  
  srand(read_cycle());
  
  stateCount = 2 + rand()%(LENGHT-2);
  direction = rand()%4;

  for(int i = 0; i < stateCount; i++) type[i] = rand()%2;
  for(int i = 0; i < stateCount; i++) stateCols[i] = rand();
  
}

void loop(){

  for(int i = 0; i < 500; i++) {

    move();
    turn(type[(state[antX][antY]%stateCount)]);
    state[antX][antY]++;
    col[antX+antY*WIDTH] = stateCols[(state[antX][antY]%stateCount)];

  }
      
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}

void turn(int angle){
  
  if(angle == 0){
    if(direction == 0){
      direction = 3;
    } else {
      direction--;
    }
  } else {
    if(direction == 3){
      direction = 0;
    } else {
      direction++;
    }
  }
}

void move(){
  
  if(antY == 0 && direction == 0){
    antY = HEIGHT-1;
  } else {
    if(direction == 0 ){
      antY--;
    }
  }
  if(antX == WIDTH-1 && direction == 1){
    antX = 0;
  } else {
    if(direction == 1){
      antX++;
    }
  }
  if(antY == HEIGHT-1 && direction == 2){
   antY = 0; 
  } else {
    if(direction == 2){
      antY++;
    }
  }
  if(antX == 0 && direction == 3){
    antX = WIDTH-1;
  } else {
    if(direction == 3){
      antX--;
    }
  }
}
