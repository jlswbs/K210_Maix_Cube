// Double Scroll chaotic oscillator //

#include <Sipeed_ST7789.h>
#include "lcd.h"

#define WIDTH 240
#define HEIGHT 240
#define SCR (WIDTH * HEIGHT)

SPIClass spi_(SPI0);
Sipeed_ST7789 lcd(WIDTH, HEIGHT, spi_);

  uint16_t col[SCR];

  int iterations = 10000;

class DoubleScroll
{
public:
  DoubleScroll(float c1);
  virtual ~DoubleScroll();
  void update(float &vc1, float &iL, float &vc2);
private:
  float g(float vc);
  float m_vc1, m_vc2, m_iL;
  float m_c1;
  const float m_c2 = 1.0f;
  const float m_L = 1.0f/7;
  const float m_G = 0.7f;
  const float m_dt = 0.001f;
};

DoubleScroll::DoubleScroll(float c1) : m_c1(c1)
{
  m_vc1 = 0.0f;
  m_vc2 = 0.1f;
  m_iL = 0.0f;
}

DoubleScroll::~DoubleScroll() {}

float DoubleScroll::g(float vc)
{
  if (vc < 1.0f && -1.0f < vc)
    return vc * -0.8f;
  else if (vc > 0)
    return -0.8f -0.5f*(vc - 1.0f);
  else
    return 0.8f + 0.5f*(-vc - 1.0f);
}

void DoubleScroll::update(float &vc1, float &iL, float &vc2)
{
  float dvc1 = m_dt*(1/m_c1)*(m_G*(m_vc2 - m_vc1) - g(m_vc1));
  float dvc2 = m_dt*(1/m_c2)*(m_G*(m_vc1 - m_vc2) + m_iL);
  float diL = m_dt*(1/m_L)* (-m_vc2);
  m_vc1 += dvc1;
  m_vc2 += dvc2;
  m_iL += diL;
  vc1 = m_vc1;
  iL = m_iL;
  vc2 = m_vc2;
}
 
DoubleScroll ds(1.0f / 9.0f);
  

void setup(){
  
  lcd.begin(15000000, COLOR_BLACK);
  lcd.setRotation(2);
  tft_write_command(INVERSION_DISPALY_ON);

  srand(read_cycle());
     
}

void loop(){

  memset(col,0,2*SCR);

  for (int i = 0; i < iterations; i++)  {

    float x, y, z;
    ds.update(y, x, z);
    uint8_t xx = (WIDTH/2) + ((WIDTH/5) * y);
    uint8_t yy = (HEIGHT/2) + (HEIGHT * z);

    if(xx > 0 && xx < WIDTH && yy > 0 && yy < HEIGHT) col[xx+yy*WIDTH] = COLOR_WHITE;
    
  }
      
  lcd.drawImage(0, 0, WIDTH, HEIGHT, (uint16_t*)col);

}
