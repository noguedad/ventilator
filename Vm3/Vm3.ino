#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <Adafruit_SSD1306.h>

Adafruit_ADS1115 ads;      // Modelo de ADS

#define SCREEN_WIDTH   128 // OLED display ancho
#define SCREEN_HEIGHT   64 // OLED display alto
#define OLED_RESET       4 // Pin de reset (NO USAR)
#define OLED_ADDR     0x3C // Direccion del display

// Declaracion de un display SSD1306 conectado a I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define NPA_ADDR      0x28  // Direccion I2C del sensor de presion
#define NPA_DELAY       21  // Retardo de lectura del sensor de presion (ms)

//            __|__
//
//    PIN ----°   °----- GND

#define SET_BTN         7   // Boton Home
#define CW_BTN          6   // Boton -->
#define CCW_BTN         5   // Boton <--

#define SIGNAL_A       12   // Señal A
#define SIGNAL_B       13   // Señal B

// Pulso señal para CW
#define CW_A            0
#define CW_B            1

// Pulso señal para CCW
#define CCW_A           1
#define CCW_B           0

#define SIGNAL_DELAY  500   // Duracion de la señal (ms)

// Posicion relativa
int Position = 0;

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Hola!");

  pinMode(SIGNAL_A, OUTPUT);
  pinMode(SIGNAL_B, OUTPUT);
  
  digitalWrite(SIGNAL_A, 0);
  digitalWrite(SIGNAL_B, 0);
  
  pinMode(SET_BTN, INPUT_PULLUP);
  pinMode(CW_BTN,  INPUT_PULLUP);
  pinMode(CCW_BTN, INPUT_PULLUP);

  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) { // Verificar pantalla
    Serial.println(F("SSD1306 Pantalla no detectada"));
    for(;;); // No pasar de aqui si no se detecta
  }

  // Limpiar el buffer
  display.clearDisplay();
  display.setTextSize(1);               // Normal 1:1 
  display.setTextColor(SSD1306_WHITE);  // Color de texto blanco (no es necesario pero \_o_/
  display.setCursor(0, 0);              // Iniciar en la esquina
  display.cp437(true);                  // Utilizar ASCII 'Code Page 437' 
  display.display();

  Serial.println("Leyendo del sensor ADS1115");
  Serial.println("Rango: +/- 4.096V (1 bit = 0.125mV)");
 
  ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 0.125mV

  ads.begin();
  Wire.begin();
}


void loop(void) 
{
  // Check Buttons
  if(digitalRead(SET_BTN) == LOW)
  {
    Position = 0;
  }
  
  if(digitalRead(CW_BTN) == LOW)
  {
      digitalWrite(SIGNAL_A, CW_A);
      digitalWrite(SIGNAL_B, CW_B);    
      delay(SIGNAL_DELAY);
      digitalWrite(SIGNAL_A, 0);
      digitalWrite(SIGNAL_B, 0);
      Position +=1;
  }
  
  if(digitalRead(CCW_BTN) == LOW)
  {
      digitalWrite(SIGNAL_A, CCW_A);
      digitalWrite(SIGNAL_B, CCW_B);    
      delay(SIGNAL_DELAY);
      digitalWrite(SIGNAL_A, 0);
      digitalWrite(SIGNAL_B, 0);
      Position -=1;
  }
  // Variables de lectura
  float adc0, adc1, adc2, adc3;
  int16_t ad0, ad1, ad2, ad3;
  char aa, bb;
  float pressure, lpressure;

  // Inicializacion de variables
  ad0 = 0; ad1 = 0; ad2 = 0;
  adc0 = 0; adc1 = 0; adc2 = 0;
  lpressure =0;

  //Tomar 10 lecturas
  for(int i = 0; i < 10; i++)
  {
    // Lecturas en el ADS
    ad0 = ads.readADC_SingleEnded(0);
    ad1 = ads.readADC_SingleEnded(1);
    ad2 = ads.readADC_SingleEnded(2);
    
    Serial.print("AIN0: "); Serial.println(ad0);
    Serial.print("AIN1: "); Serial.println(ad1);
    Serial.print("AIN2: "); Serial.println(ad2);
  
    // Comparar valores y guardar el mayor
    if(ad0 * 0.125 > adc0)
      adc0 = ad0 * 0.125;
    if(ad1 * 0.125 > adc1)
      adc1 = ad1 * 0.125;
    if(ad2 * 0.125 > adc2)
      adc2 = ad2 * 0.125;  

    // Pedir valor al sensor de presion
    Wire.beginTransmission(NPA_ADDR);
    Wire.write(0xAC);  Wire.write(0);
    Wire.endTransmission();
    delay(NPA_DELAY);
    Wire.requestFrom(NPA_ADDR, 2);  // Viene en 2 bytes: 14 bits
    aa = Wire.read();
    bb = Wire.read();
    pressure =  (float) ((aa << 8) | bb); // Juntar los 2 bytes para formar un valor float de los 14 bits
    pressure = pressure * (0.214 / 2);    //Conversion a mmH2O
    // Comparar y guardar el valor mayor
    if(pressure > lpressure)
      lpressure = pressure;
    
    Serial.print("Press: "); Serial.println(pressure);
  }

  // Limpiar pantalla
  display.clearDisplay();
  display.setCursor(0, 0);
  display.display();

  // Mostrar V o mV
  if(adc0 > 1000){
    display.println("O2: "); display.print(adc0/1000); display.print("V");}
  else{
  display.println("O2: "); display.print(adc0); display.print("mV");}

  //Conversion de O2
  display.print(" "); display.print(adc0*1.68); display.println("%");

  // Mostrar V o mV
  if(adc1 > 1000){
    display.println("Flujo: "); display.print(adc1/1000); display.print("V");}
  else{
  display.println("Flujo: "); display.print(adc1); display.print("mV");}

  // Conversion Flujo
  display.print(" "); display.print(adc1/246); display.println("");

  // Mostrar V o mV
  if(adc2 > 1000){
    display.println("Temp: "); display.print(adc2/1000); display.print("V");}
  else{
  display.println("Temp: "); display.print(adc2); display.print("mV");}
  
  // Conversion Temp
  display.print(" "); display.print(adc2*0.027); display.println("C");

  // Mostrar Presion
  display.println("Presion: "); display.print(lpressure); display.print("mmH2O");

  // Convertir a PSI
  display.print(" "); display.print(lpressure/703.07); display.println("psi");

   display.setCursor(50, 0); display.print("P:"); display.print(Position);
   
  display.display();
}
