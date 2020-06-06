#include <U8x8lib.h>
#include <Adafruit_ADS1015.h>
#include <Adafruit_EEPROM_I2C.h>

#define ADC_GAIN    0.125
#define EEPROM_ADDR 0x50

Adafruit_ADS1115 ads;
Adafruit_EEPROM_I2C eeprom;
U8X8_SSD1306_128X64_NONAME_HW_I2C screenA(U8X8_PIN_NONE);

int   cstatus = 0;
float O2mV;
float FlowV;
float TempV;

uint8_t FlowSerial[6];
uint32_t FlowModel;
char     FlowRev;
uint16_t FYear;
uint8_t  FMonth;
uint8_t  FDay;
float    Tcal;
float    S;
float    Z;
float    Tcorr;
uint8_t  CalA;
uint8_t  CalO;

void setup() {
  // put your setup code here, to run once:
  screenA.begin();
  screenA.clearDisplay();
  screenA.setFont(u8x8_font_7x14_1x2_r);
  screenA.drawString( 3, 0,"Grupo APM");  
  screenA.setFont(u8x8_font_5x7_r);
  screenA.setCursor( 0, 2);
  screenA.println("SELF POST TEST");
  screenA.println("SCREEN A: OK");
  screenA.print("ADS INIT: ");
  ads.setGain(GAIN_ONE); //1 bit = 0.125mV
  ads.begin();
  screenA.println("OK");
  screenA.print("FLOW S: ");
  if(eeprom.begin(EEPROM_ADDR))
    screenA.println("OK");
  else
    screenA.println("NA");
  delay(2000);
  for(int i = 0; i < 6; i++)
  {
    FlowSerial[i] = eeprom.read8(2 + 1);  
  }
  screenA.clearDisplay();
  screenA.setCursor(0,0);
  screenA.println("Flow Sensor");
  screenA.println("Serial:");
  for(int i = 0; i < 6; i++)
  {
    screenA.print(FlowSerial[i], DEC);
  }
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  switch(cstatus)
  {
    case 0: sampleADC();
            screenA.clearDisplay();
            screenA.setCursor(0,0);
            screenA.print("O2: "); screenA.print(O2mV); screenA.println("mV");
            screenA.print("Flow: "); screenA.print(FlowV); screenA.println("V");
            screenA.print("Temp: "); screenA.print(TempV); screenA.println("V");  
            break;
  }
  delay(500);
}

void sampleADC()
{
    int16_t ad0, ad1, ad2, ad3;
    int16_t lad0, lad1, lad2, lad3;
    ad0 = 0; ad1 = 0; ad2 = 0; ad3 = 0;
    lad0 = 0; lad1 = 0; lad2 = 0; lad3 = 0;

    for(int i = 0; i < 100; i++)
    {
      ad0 = ads.readADC_SingleEnded(0);      
      if(ad0 > lad0)
        lad0 = ad0;
      ad1 = ads.readADC_SingleEnded(1);
      if(ad1 > lad1)
        lad1 = ad1;
      ad2 = ads.readADC_SingleEnded(2);
      if(ad2 > lad2)
        lad2 = ad2;
      ad3 = ads.readADC_SingleEnded(3);
      if(ad3 > lad3)
        lad3 = ad3;
    }
    O2mV = ad0 * ADC_GAIN;
    FlowV = ad2 * ADC_GAIN / 1000;
    TempV = ad3 * ADC_GAIN / 1000;
}
