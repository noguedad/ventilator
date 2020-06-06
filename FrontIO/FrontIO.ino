 /*****
 * IO Module
 *****/

#include <ssd1306.h>

#define BTN_DELAY   250
#define SEL_DELAY   3      

#define O2_SET  7
#define O2_CLK  5
#define O2_DT   6
#define O2_MAX  100
#define O2_MIN  21

#define BPM_SET  10
#define BPM_CLK  8
#define BPM_DT   9
#define BPM_MAX  60
#define BPM_MIN  4

int O2      =   0;
int lO2clk  =   0;
int cO2clk  =   0;
int setO2   =   O2_MIN;
unsigned long lastO2 = 0;
bool flagO2  = false;

int BPM     =   0;
int lBPMclk =   0;
int cBPMclk =   0;
int setBPM  =   BPM_MIN;
unsigned long lastBPM = 0;
bool flagBPM = false;

//=INICIALIZACION=
void setup() 
{
  Serial.begin(9600);
  Serial.println("Inicializacion RS232");
  Serial.println("Inicializacion OLED");
  ssd1306_128x64_i2c_init();
  ssd1306_setFixedFont(ssd1306xled_font6x8);
  ssd1306_clearScreen();
  ssd1306_printFixed(0,  0, "Grupo APM (c) 2020", STYLE_NORMAL);
  delay(1000);

  Serial.println("Configurando Entradas");
  pinMode(O2_SET, INPUT_PULLUP);
  pinMode(O2_CLK, INPUT);
  pinMode(O2_DT, INPUT);
  pinMode(BPM_SET, INPUT_PULLUP);
  pinMode(BPM_CLK, INPUT);
  pinMode(BPM_DT, INPUT);

  Serial.println("Dibujo Inicial");
    InitialDraw();
    DrawNormalValue(0, 20, O2);
    DrawNormalValue(0, 50, BPM);
    DrawNormalValue(65, 20, setO2);
    DrawNormalValue(65, 50, setBPM);
  
  Serial.println("Obteniendo IO Inicial");
  lO2clk = digitalRead(O2_CLK);
  lBPMclk = digitalRead(BPM_CLK);
}

//=PROGRAMA=
void loop() 
{
  cO2clk = digitalRead(O2_CLK);
  cBPMclk = digitalRead(BPM_CLK);
  char strBuf[3];
  sprintf(strBuf, "%02d", lO2clk); DrawText(70, 0, strBuf);
  sprintf(strBuf, "%02d", cO2clk); DrawText(90, 0, strBuf);
  
  if(cO2clk != lO2clk)
  {
      lO2clk = cO2clk;
      if(digitalRead(O2_DT) != cO2clk)
      {
            if(setO2 < O2_MAX && flagO2)
            setO2 ++;
       
      }
      else
      { 
        if(setO2 > O2_MIN && flagO2)
            setO2 --;
    
      }
      DrawInvertedValue(65, 20, setO2);
      
  }

  if(cBPMclk != lBPMclk && flagBPM)
  {
      lBPMclk = cBPMclk;
      if(digitalRead(BPM_DT) != cBPMclk)
      {
        if(setBPM < BPM_MAX)
            setBPM ++;
      }
      else
      {
        if(setBPM > BPM_MIN)
            setBPM --;
      }
      DrawInvertedValue(65, 50, setBPM);
  }
      
  if(!digitalRead(O2_SET))
  {
    flagO2 = !flagO2;
    if(flagO2)
    {
      Serial.println("O2 SET ON");
      DrawInvertedValue(65, 20, setO2);
    }
    else
    {
      Serial.println("O2 SET OFF");
      DrawNormalValue(65, 20, setO2);
    }
    delay(BTN_DELAY);
  }
  
  if(!digitalRead(BPM_SET))
  {
    flagBPM = !flagBPM;
    if(flagBPM)
    {
      Serial.println("BPM SET ON");
      DrawInvertedValue(65, 50, setBPM);
    }
    else
    {
      Serial.println("BPM SET OFF");
      DrawNormalValue(65, 50, setBPM);
    }
    delay(BTN_DELAY);
  }
  delay(SEL_DELAY);
}

void DrawText(int column, int row, char* text)
{
  ssd1306_setFixedFont(ssd1306xled_font6x8);
  ssd1306_printFixed(column, row, text, STYLE_NORMAL);
}

void DrawNormalValue(int column, int row, int value)
{
  char strBuf[5];
  sprintf(strBuf, "%03d", value);
  ssd1306_setFixedFont(courier_new_font11x16_digits);
  ssd1306_printFixed(column, row, strBuf, STYLE_BOLD);
}

void DrawInvertedValue(int column, int row, int value)
{
  char strBuf[5];
  sprintf(strBuf, "%03d", value);
  ssd1306_negativeMode();
  ssd1306_setFixedFont(courier_new_font11x16_digits);
  ssd1306_printFixed(column, row, strBuf, STYLE_BOLD);
  ssd1306_positiveMode();
}

void InitialDraw()
{
  ssd1306_setFixedFont(ssd1306xled_font6x8);
  ssd1306_clearScreen();
  ssd1306_printFixed(  0, 10, "FiO2:", STYLE_NORMAL);
  ssd1306_printFixed( 35, 20, "%", STYLE_NORMAL);  
  ssd1306_printFixed( 35, 25, "READ", STYLE_NORMAL);  
  ssd1306_printFixed(100, 20, "%", STYLE_NORMAL);  
  ssd1306_printFixed(100, 25, "SET", STYLE_NORMAL);  
  ssd1306_printFixed(  0, 40, "RESPIRACION:", STYLE_NORMAL);
  ssd1306_printFixed( 35, 55, "bpm", STYLE_NORMAL);  
  ssd1306_printFixed(100, 55, "bpm", STYLE_NORMAL);  
  ssd1306_printFixed( 35, 60, "READ", STYLE_NORMAL);
  ssd1306_printFixed(100, 60, "SET", STYLE_NORMAL);
}
