//* LIBRERIAS *
#include <U8x8lib.h>

//* DEFINICIONES *
#define CLK_O2    2
#define DT_O2     3
#define SW_O2     4

#define CLK_BPM   5
#define DT_BPM    6
#define SW_BPM    7

#define CLK_PEEP  8
#define DT_PEEP   9
#define SW_PEEP  10

#define CLK_PIP  11
#define DT_PIP   12
#define SW_PIP   13

#define READ      0
#define SET      10
#define TOP       1
#define BOTTOM    5


//* CLASES *
U8X8_SSD1306_128X64_NONAME_SW_I2C screenA(A1, A0);
U8X8_SSD1306_128X64_NONAME_SW_I2C screenB(A3, A2);
U8X8_SSD1306_128X64_NONAME_SW_I2C screenC(A5, A4);

void drawInitialScreen(void);
void drawValue(U8X8_SSD1306_128X64_NONAME_SW_I2C &screen, int x, int y, int value, bool inverted = false);

void setup()
{
  screenA.begin();
  screenB.begin();
  screenC.begin();
  screenA.clearDisplay();
  screenB.clearDisplay();
  screenC.clearDisplay();
  screenA.setFont(u8x8_font_7x14_1x2_r);
  screenB.setFont(u8x8_font_7x14_1x2_r);
  screenC.setFont(u8x8_font_7x14_1x2_r);
  screenA.drawString(4,20,"Grupo APM");
  screenB.drawString(4,20,"Grupo APM");
  screenC.drawString(4,20,"Grupo APM");
  delay(100);
  
  screenA.setFont(u8x8_font_5x7_r);
  screenB.setFont(u8x8_font_5x7_r);
  screenC.setFont(u8x8_font_5x7_r);  
  screenA.drawString(5,22,"(C) 2020");
  screenB.drawString(5,22,"(C) 2020");
  screenC.drawString(5,22,"(C) 2020");
  delay(100);
  
  screenA.setFont(u8x8_font_open_iconic_check_4x4);
  screenB.setFont(u8x8_font_open_iconic_check_4x4);
  screenC.setFont(u8x8_font_open_iconic_check_4x4);
  screenA.drawString(6,0,"A");
  screenB.drawString(6,0,"A");
  screenC.drawString(6,0,"A");
  delay(100);
  drawInitialScreen();
  drawValue(screenA, READ, TOP,  7);
  drawValue(screenA, READ, BOTTOM,  9);
  drawValue(screenB, READ, TOP,  6);
  drawValue(screenB, READ, BOTTOM,100);

  drawValue(screenA, SET, TOP, 10);
  drawValue(screenA, SET, BOTTOM,  5);
  drawValue(screenB, SET, TOP, 12);
  drawValue(screenB, SET, BOTTOM,100);
}

void loop()
{

}

void drawValue(U8X8_SSD1306_128X64_NONAME_SW_I2C &screen, int x, int y, int value, bool inverted)
{
  char sValue[5];
  sprintf(sValue, "%d", value);
  screen.setInverseFont(inverted);
  screen.setFont(u8x8_font_profont29_2x3_n);
  if(value > 99)
    screen.setCursor(x,y);
  else if(value > 9)
    screen.setCursor(x+2,y);
  else
    screen.setCursor(x+4, y);
  screen.print(sValue);
}

void drawInitialScreen()
{
  screenA.clear();
  screenB.clear();
  screenC.clear();

  screenA.setFont(u8x8_font_artossans8_r);
  screenB.setFont(u8x8_font_artossans8_r);
  screenC.setFont(u8x8_font_artossans8_r);

  screenA.drawString( 0, 0,"PEEP:");
  screenA.drawString( 0,20,"PIP:");
  screenB.drawString( 0, 0,"BREATH FRQ:");
  screenB.drawString( 0,20,"FiO2:");

  screenA.setFont(u8x8_font_5x7_f);
  screenB.setFont(u8x8_font_5x7_f);
  screenA.drawString( 10, 0,"cmH2O");
  screenA.drawString( 10,20,"%FiO2");
  screenB.drawString( 12, 0,"bpm");
  screenB.drawString( 14,20,"%");

  screenA.setFont(u8x8_font_open_iconic_arrow_2x2);
  screenB.setFont(u8x8_font_open_iconic_arrow_2x2);
  screenA.drawString( 7, 1,"F");
  screenA.drawString( 7, 5,"F");
  screenB.drawString( 7, 1,"F");
  screenB.drawString( 7, 5,"F");
  
  
}
