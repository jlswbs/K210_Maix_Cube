// Sierpinski triangle fractal //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

  uint16_t col[SCR];

struct point{
  int x;
  int y;
};

  point points[10];
  point current;
  
  int iterations = 10000;

void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);

  srand(read_cycle());
  
  for(int i=0; i<10; i++){
    points[i].x = (WIDTH/2) * sin(2*PI*i/3);
    points[i].y = (HEIGHT/2) * cos(2*PI*i/3);
  }
    
  current = points[rand()%10];
     
}

void loop(){

  for (int i = 0; i < 10; i++) {
 
    point nextpoint = points[rand()%10]; 
    current.x = (current.x + nextpoint.x)/2;
    current.y = (current.y + nextpoint.y)/2;

    uint8_t xx = (WIDTH/2) + current.x;
    uint8_t yy = -25 + (HEIGHT/2) + current.y;
       
    if(xx > 0 && xx < WIDTH && yy > 0 && yy < HEIGHT) col[xx+yy*WIDTH] = COLOR_WHITE;

  }
      
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
