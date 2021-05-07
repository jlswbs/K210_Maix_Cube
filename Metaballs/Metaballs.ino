// Metaballs demo //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

  uint16_t col[SCR];

#define NUM_BALLS     5
#define MIN_VELOCITY  3
#define MAX_VELOCITY  5
#define MIN_RADIUS    10
#define MAX_RADIUS    12

uint16_t coll;

struct vector2 {
    int16_t x;
    int16_t y;
};

struct ball {
    struct vector2 position;
    struct vector2 velocity;
    uint16_t radius;
};

struct ball balls[NUM_BALLS];

void metaballs_init(){
  
  for (int i = 0; i < NUM_BALLS; i++) {
    balls[i].radius = (rand() % MAX_RADIUS) + MIN_RADIUS;
    balls[i].position.x = rand() % WIDTH;
    balls[i].position.y = rand() % HEIGHT;
    balls[i].velocity.x = (rand() % MAX_VELOCITY) + MIN_VELOCITY;
    balls[i].velocity.y = (rand() % MAX_VELOCITY) + MIN_VELOCITY;
  }
  
}

void metaballs_animate(){
  
  for (int i = 0; i < NUM_BALLS; i++) {
    balls[i].position.x += balls[i].velocity.x;
    balls[i].position.y += balls[i].velocity.y;

    if ((balls[i].position.x < 0) | (balls[i].position.x > WIDTH)) balls[i].velocity.x = balls[i].velocity.x * -1;
    if ((balls[i].position.y < 0) | (balls[i].position.y > HEIGHT)) balls[i].velocity.y = balls[i].velocity.y * -1;
  }
  
}

void metaballs_render(){  

  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      float sum = 0;
      for (int i = 0; i < NUM_BALLS; i++) {
        float dx = x - balls[i].position.x;
        float dy = y - balls[i].position.y;
        float d2 = dx * dx + dy * dy;
        sum += balls[i].radius * balls[i].radius / d2;
      }
      
      if (sum > 0.75f) coll = COLOR_BLUE;
      else if (sum > 0.5f) coll = COLOR_WHITE;
      else if (sum > 0.35f) coll = COLOR_YELLOW;
      else coll = COLOR_BLACK;
      
      col[x+y*WIDTH] = coll;
            
    }
  }
}

void setup(){

  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);
  
  srand(read_cycle());
  
  metaballs_init();
  
}

void loop(){

  metaballs_animate();
  metaballs_render();
      
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
