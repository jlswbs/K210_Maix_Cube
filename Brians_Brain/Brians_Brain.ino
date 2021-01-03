// Brian's Brain Cellular Automata //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

#define DENSITY     7
#define READY       0
#define REFRACTORY  1
#define FIRING      2

  uint16_t col[SCR];

  int world[WIDTH][HEIGHT];
  int temp[WIDTH][HEIGHT];


int weighted_randint(int true_weight){
  
    int choice = rand() % 10;
    
    if (choice > true_weight) return 1;
    else return 0;
}


int count_neighbours(int world[WIDTH][HEIGHT], int x_pos, int y_pos){
  
    int x, y, cx, cy, cell;
    int count = 0;

    for (y = -1; y < 2; y++) {
        for (x = -1; x < 2; x++) {
            cx = x_pos + x;
            cy = y_pos + y;
            if ( (0 <= cx && cx < WIDTH) && (0 <= cy && cy < HEIGHT)) {
                cell = world[x_pos + x][y_pos + y];
                if (cell == FIRING) count ++;
            }
        }
    }
  return count;
}


void apply_rules(int world[WIDTH][HEIGHT]){
  
  int x, y, cell, neighbours;

  memcpy(temp, world, sizeof(temp));

  for (y = 0; y < HEIGHT; y++) {
    for (x = 0; x < WIDTH; x++){
      cell = temp[x][y];          
      if (cell == READY) {
        neighbours = count_neighbours(temp, x, y);
        if (neighbours == 2) world[x][y] = FIRING; }
      else if (cell == FIRING) world[x][y] = REFRACTORY;
      else world[x][y] = READY;
    }
  }
}

void populate(){
  
  int x, y, r;
  
  for (y = 0; y < HEIGHT; y++) {
    for (x = 0; x < WIDTH; x++){
      r = weighted_randint(DENSITY);
      if (r == 1) world[x][y] = FIRING;
      else world[x][y] = READY;
    }
  }
} 

void draw_world(int world[WIDTH][HEIGHT]){
    
    int x, y;
    
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++){
            if (world[x][y] == FIRING) col[x+WIDTH*y] = COLOR_WHITE;    
            else if (world[x][y] == REFRACTORY) col[x+WIDTH*y] = COLOR_WHITE;
            else col[x+WIDTH*y] = COLOR_BLACK; 
        }
    }
}
  
void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);

  srand(read_cycle());
  
  populate();
  
}

void loop(){
  
  apply_rules(world);
  draw_world(world); 
       
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
