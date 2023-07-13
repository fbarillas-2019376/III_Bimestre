/*
 * Fundacion Kinal
 * Centro educativo tecnico laboral Kinal
 * Perito en electonica
 * Quinto perito
 * EB5AM
 * Taller de electronica digital y reparacion de computadoras
 * Nombre: Francisco Barillas
 * Carnet: 2019376
*/

//Incluyo Librerias
#include <LedControl.h>
#include <Ticker.h>

//Directivas de preprocesador
#define DIN 4
#define CLK 2
#define CS 3
#define Sensor A0

//Funciones
void Salidas();
void Valor_Gas();
void Actualizacion();
void Animacion();
void Lectura();

//Constructores
LedControl LED_FRANCISCO = LedControl(4,2,3,1);
Ticker ISR_Sensor(Lectura, 5000);

//Variables
const int Largo = 8; 
int gas;
int propano; 

byte Fuego[8] = 
{
B00001000,
B00001100,
B00011100,
B00111100,
B01101110,
B01011110,
B00101100,
B00011000
};
byte Fuego2[8] = 
{
B01001001,
B01011001,
B00111110,
B01111110,
B01101110,
B01011110,
B00101100,
B00011000
};
byte Copo_Nieve[8] = 
{
B10011001,
B01011010,
B00111100,
B11111111,
B11111111,
B00111100,
B01011010,
B10011001
};

void setup() 
{
  Serial.begin(19200); 
  Salidas();
}

void loop() 
{
  Valor_Gas();
  Actualizacion();
}

//
void Salidas()
{
  ISR_Sensor.start();
  LED_FRANCISCO.shutdown(0, false); 
  LED_FRANCISCO.setIntensity(0, 15); 
  LED_FRANCISCO.clearDisplay(0); 
  pinMode(Sensor, INPUT); 
}

//
void Actualizacion()
{
  ISR_Sensor.update();
}


  //  
void Valor_Gas()
  {
  if (propano > 40) {
    Animacion();
  } else {
    for (int i = 0; i < Largo; i++) {
    LED_FRANCISCO.setColumn(0, i, Copo_Nieve[i]);
  }
  }
  delay(1000);
  }
//
void Lectura(){
  int suma =  0;
  for(int i = 0; i < 5; i++){
  gas = map(analogRead(A0),0,1023,0,100);
  suma = suma + gas;
  delay(100);
  }
  propano = suma/5;
  Serial.print(gas);
  Serial.print(",");
}

//
void Animacion()
{
  LED_FRANCISCO.clearDisplay(0);
  for(int Tiempo = 0; Tiempo < 5; Tiempo++){
  for (int i = Largo; i > 0; i--) {
  LED_FRANCISCO.setColumn(0, i, Fuego[i]);
  }
  delay(500);
  
  for (int i = Largo; i > 0; i--) {
  LED_FRANCISCO.setColumn(0, i, Fuego2[i]);
  }
  delay(500);
}
}
