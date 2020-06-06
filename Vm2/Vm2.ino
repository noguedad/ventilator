#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <Adafruit_SSD1306.h>

Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Hello!");

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.display();

  Serial.println("Getting single-ended readings from AIN0..3");
  Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");
  
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  
  ads.begin();
}

void loop(void) 
{
  float adc0, adc1, adc2, adc3;
  int16_t ad0, ad1, ad2, ad3;

ad0 = 0; ad1 = 0; ad2 = 0; ad3 = 0;
adc0 = 0; adc1 = 0; adc2 = 0; adc3 = 0;

for(int i = 0; i < 100; i++)
{
  ad0 = ads.readADC_SingleEnded(0);
  ad1 = ads.readADC_SingleEnded(1);
  ad2 = ads.readADC_SingleEnded(2);
  ad3 = ads.readADC_SingleEnded(3);
  Serial.print("AIN0: "); Serial.println(ad0);
  Serial.print("AIN1: "); Serial.println(ad1);
  Serial.print("AIN2: "); Serial.println(ad2);
  Serial.print("AIN3: "); Serial.println(ad3);
  if(ad0 * 0.125 > adc0)
    adc0 = ad0 * 0.125;
  if(ad1 * 0.125 > adc1)
    adc1 = ad1 * 0.125;
  if(ad2 * 0.125 > adc2)
    adc2 = ad2 * 0.125;
  if(ad3 * 0.125 > adc3)
    adc3 = ad3 * 0.125; 
}

display.clearDisplay();
display.setCursor(0, 0);     // Start at top-left corner
display.display();

if(adc0 > 1000){
  display.print("O2: "); display.print(adc0/1000); display.println(" V");}
else{
display.print("O2: "); display.print(adc0); display.println(" mV");}

if(adc1 > 1000){
  display.print("Flujo: "); display.print(adc1/1000); display.println(" V");}
else{
display.print("Flujo: "); display.print(adc1); display.println(" mV");}

if(adc2 > 1000){
  display.print("Temp: "); display.print(adc2/1000); display.println(" V");}
else{
display.print("Temp: "); display.print(adc2); display.println(" mV");}

if(adc3 > 1000){
  display.print("Puerto 4: "); display.print(adc3/1000); display.println(" V");}
else{
  display.print("Puerto 4: "); display.print(adc3); display.println(" mV");}

display.display();
}
