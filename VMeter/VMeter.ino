//* LIBRERIAS *
#include <U8x8lib.h>

//* CLASES *
U8X8_SSD1306_128X64_NONAME_SW_I2C screenA( 7, 6);

void drawInitialScreen(U8X8_SSD1306_128X64_NONAME_SW_I2C &screen);
void drawValue(U8X8_SSD1306_128X64_NONAME_SW_I2C &screen, int x, int y, int value, bool inverted = false);

//* CONFIGURACION INICAL *
void setup() 
{
  screenA.begin();
  screenA.clearDisplay();
  screenA.setFont(u8x8_font_7x14_1x2_r);
  screenA.drawString( 3, 0, "Grupo APM");
  screenA.drawString( 4, 2, "V-Meter");  
  screenA.setFont(u8x8_font_5x7_r);
  screenA.drawString( 0, 4, "PLUG TO PORT A1");
  screenA.drawString( 3, 6,"= = 5 = =");
  delay(250);
  screenA.drawString( 3, 6,"= = 4 = =");
  delay(250);
  screenA.drawString( 3, 6,"= = 3 = =");
  delay(250);
  screenA.drawString( 3, 6,"= = 2 = =");
  delay(250);
  screenA.drawString( 3, 6,"= = 1 = =");
  delay(250);
  screenA.clearDisplay();
   screenA.setFont( u8x8_font_inb21_2x4_r);
}

long int l = 0;
int i = 0;
//* PROGRAMA PRINCIPAL *
void loop() 
{
  if(i < 100)
  {
    int x = analogRead(A1);;
    if(x > l)
     l = x;
    i ++;
  }
  else
  {
  float v = 4.7 * l ;
  if(v >= 1000)
  {
    v =  v / 1000;
    screenA.drawString( 10, 3, " V");
  }
  else
  {
    screenA.drawString( 10, 3, "mV");    
  }
    screenA.setCursor(0,0);
    if(v < 10)
      screenA.print("  ");      
    if(v < 100)
      screenA.print(" ");      
    screenA.print(v);
  i=0;
  l=0;
  delay(1);
  }
}
