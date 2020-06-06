//* LIBRERIAS *
#include <U8x8lib.h>

//* CLASES *
U8X8_SSD1306_128X64_NONAME_SW_I2C screenA( 7, 6);
U8X8_SSD1306_128X64_NONAME_SW_I2C screenB( 9, 8);

void drawInitialScreen(U8X8_SSD1306_128X64_NONAME_SW_I2C &screen);
void drawValue(U8X8_SSD1306_128X64_NONAME_SW_I2C &screen, int x, int y, int value, bool inverted = false);

//* CONFIGURACION INICAL *
void setup() 
{
  screenA.begin();
  screenB.begin();

  screenA.clearDisplay();
  screenB.clearDisplay();

  screenA.setFont(u8x8_font_7x14_1x2_r);
  screenB.setFont(u8x8_font_7x14_1x2_r);

  screenA.drawString( 3, 0,"Grupo APM");
  screenB.drawString( 3, 0,"Grupo APM");  
  
  screenA.setFont(u8x8_font_5x7_r);
  screenB.setFont(u8x8_font_5x7_r);

  screenA.setCursor( 0, 2);
  screenB.setCursor( 0, 2);
  screenA.println("SELF POST TEST");
  screenB.println("SELF POST TEST");
  screenA.println("SCREEN A: OK");
  screenB.println("SCREEN B: OK");
}

//* PROGRAMA PRINCIPAL *
void loop() 
{

}
