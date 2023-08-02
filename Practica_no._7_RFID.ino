/*
   Fundacion Kinal
   Centro educativo tecnico laboral Kinal
   Electronica
   Grado: Quinto
   Sección: A
   Curso: Taller de electronica digital y reparación de computadoras I
   Nombre: Francisco Barillas
   Carnet: 2019376
*/
//Icluyo Librerias
#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <DS3231.h>
#include <LiquidCrystal_I2C.h> 

//Directivas de preprocesador
#define Direccion_LCD 0x27
#define FILAS 2
#define COLUMNAS 16
#define RST_PIN  9   
#define SS_PIN  10  
#define pinvoltaje A0   
#define corriente A7 
#define configRTC 6  

//Variables
float Sensibilidad = 0.185;
byte Year ;
byte Month ;
byte Date ;
byte DoW ;
byte Hour ;
byte Minute ;
byte Second ;
bool Century  = false;
bool h12 ;
bool PM ;
byte ActualUID[4]; 
byte Usuario2[4]= {0x79, 0x02, 0x9B, 0xD5} ; 
byte Usuario1[4]= {0xF9, 0x90, 0xF8, 0xB3} ;

//Constructores
MFRC522 Reloj(SS_PIN, RST_PIN); 
DS3231 Clock;
LiquidCrystal_I2C LCD_FRANCISCO_BARILLAS(Direccion_LCD, COLUMNAS, FILAS);

void setup() {
  Wire.begin();
  Serial.begin(9600); 
  SPI.begin();      
  Reloj.PCD_Init(); 
  pinMode(pinvoltaje, INPUT);
  pinMode(corriente, INPUT);
  pinMode(configRTC, INPUT);
  LCD_FRANCISCO_BARILLAS.init();
  LCD_FRANCISCO_BARILLAS.backlight();
  if(digitalRead(configRTC) == HIGH){
    setDate();
    }
    LCD_FRANCISCO_BARILLAS.setCursor(0,0);
    LCD_FRANCISCO_BARILLAS.print(" EEGSA - KINAL  ");
    LCD_FRANCISCO_BARILLAS.setCursor(0,1);
    LCD_FRANCISCO_BARILLAS.print("                ");
}
void loop() {
  if ( Reloj.PICC_IsNewCardPresent()) 
{  
  if ( Reloj.PICC_ReadCardSerial()){
  for (byte i = 0; i < Reloj.uid.size; i++) {
  ActualUID[i]=Reloj.uid.uidByte[i];          
} 
 Serial.print("     ");                  
 if(compareArray(ActualUID,Usuario1)){
 Serial.println("Verificado: Francisco Barillas");
 Serial.print("Fecha de acceso: ");
 Serial.print(Clock.getDate(), DEC);
 Serial.print("/");
 Serial.print(Clock.getMonth(Century), DEC);
 Serial.print(" ");
 Serial.print(Clock.getHour(h12, PM), DEC);
 Serial.print(":");
 Serial.print(Clock.getMinute(), DEC);
 Serial.print(":");
 Serial.println(Clock.getSecond(), DEC);
 calculos();
                    }
 else if(compareArray(ActualUID,Usuario2)){
 Serial.println("Verificado: Sebas Ixen");
 Serial.print("Fecha de acceso: ");
 Serial.print(Clock.getDate(), DEC);
 Serial.print("/");
 Serial.print(Clock.getMonth(Century), DEC);
 Serial.print(" ");
 Serial.print(Clock.getHour(h12, PM), DEC);
 Serial.print(":");
 Serial.print(Clock.getMinute(), DEC);
 Serial.print(":");
 Serial.println(Clock.getSecond(), DEC);
 calculos();
}
else{
  Serial.println("Acceso fallido   ");
  LCD_FRANCISCO_BARILLAS.setCursor(0,1);
  LCD_FRANCISCO_BARILLAS.print("NO REGISTRADO   ");
  Serial.print("Fecha de intento de acceso: ");
  Serial.print(Clock.getDate(), DEC);
  Serial.print("/");
  Serial.print(Clock.getMonth(Century), DEC);
  Serial.print(" ");
  Serial.print(Clock.getHour(h12, PM), DEC); //24-hr
  Serial.print(":");
  Serial.print(Clock.getMinute(), DEC);
  Serial.print(":");
  Serial.println(Clock.getSecond(), DEC);
  delay(2000);
}
 Reloj.PICC_HaltA();          
}         
}
 else{
  LCD_FRANCISCO_BARILLAS.setCursor(0,0);
  LCD_FRANCISCO_BARILLAS.print(" EEGSA - KINAL  ");
  LCD_FRANCISCO_BARILLAS.setCursor(0,1);
        LCD_FRANCISCO_BARILLAS.print("MEDIDOR ENERGIA  ");             
}
}
 boolean compareArray(byte array1[],byte array2[])
{
  if(array1[0] != array2[0])return(false);
  if(array1[1] != array2[1])return(false);
  if(array1[2] != array2[2])return(false);
  if(array1[3] != array2[3])return(false);
  return(true);
}
void calculos(){
  LCD_FRANCISCO_BARILLAS.setCursor(0,1);
  LCD_FRANCISCO_BARILLAS.print("                ");
  LCD_FRANCISCO_BARILLAS.setCursor(1,1);
  LCD_FRANCISCO_BARILLAS.print("Voltaje ");
  float voltaje =  (float)25*analogRead(pinvoltaje)/1023;
  LCD_FRANCISCO_BARILLAS.print(voltaje);
  LCD_FRANCISCO_BARILLAS.print(" ");//  Voltaje
  delay(5000); 
  LCD_FRANCISCO_BARILLAS.setCursor(0,1);
  LCD_FRANCISCO_BARILLAS.print("Corriente ");
  float med = analogRead(corriente)*(5.0/1023.0);
  float I = abs((2.5 - med)/Sensibilidad);
  if(abs(I) >= 1 ){
  LCD_FRANCISCO_BARILLAS.print(I,3);
  LCD_FRANCISCO_BARILLAS.print(" ");
  }
  if(abs(I) < 1 ){
  LCD_FRANCISCO_BARILLAS.print(I*1000,0);
  LCD_FRANCISCO_BARILLAS.print(" ");//  Corriente
  }
  delay(5000);
  LCD_FRANCISCO_BARILLAS.setCursor(0,1);
  float P = voltaje * I;
  LCD_FRANCISCO_BARILLAS.print("Potencia ");
  if(abs(P) >= 1 ){
  LCD_FRANCISCO_BARILLAS.print(I,3);
  LCD_FRANCISCO_BARILLAS.print(" ");
  }
  if(abs(P) < 1 ){
  LCD_FRANCISCO_BARILLAS.print(P*1000,0);
  LCD_FRANCISCO_BARILLAS.print(" ");//  Potencia
  }
  delay(5000);
  }
  void setDate( ) {
  if (digitalRead(configRTC) == HIGH) {
    GetDateStuff(Year, Month, Date, DoW, Hour, Minute, Second);
    Clock.setClockMode(false);  
    Clock.setSecond(Second);
    Clock.setMinute(Minute);
    Clock.setHour(Hour);
    Clock.setDate(Date);
    Clock.setMonth(Month);
    Clock.setYear(Year);
    Clock.setDoW(DoW);
  }
}
void GetDateStuff(byte& Year, byte& Month, byte& Day, byte& DoW, byte& Hour, byte& Minute, byte& Second) { /* function GetDateStuff */
  boolean GotString = false;
  char InChar;
  byte Temp1, Temp2;
  char InString[20];
  byte j = 0;
  while (!GotString) {
    if (Serial.available()) {
      InChar = Serial.read();
      InString[j] = InChar;
      j += 1;
      if (InChar == 'x') {
        GotString = true;
      }
    }
  }
  Serial.println(InString);
  // Read Year first
  Temp1 = (byte)InString[0] - 48;
  Temp2 = (byte)InString[1] - 48;
  Year = Temp1 * 10 + Temp2;
  // now month
  Temp1 = (byte)InString[2] - 48;
  Temp2 = (byte)InString[3] - 48;
  Month = Temp1 * 10 + Temp2;
  // now date
  Temp1 = (byte)InString[4] - 48;
  Temp2 = (byte)InString[5] - 48;
  Day = Temp1 * 10 + Temp2;
  // now Day of Week
  DoW = (byte)InString[6] - 48;
  // now Hour
  Temp1 = (byte)InString[7] - 48;
  Temp2 = (byte)InString[8] - 48;
  Hour = Temp1 * 10 + Temp2;
  // now Minute
  Temp1 = (byte)InString[9] - 48;
  Temp2 = (byte)InString[10] - 48;
  Minute = Temp1 * 10 + Temp2;
  // now Second
  Temp1 = (byte)InString[11] - 48;
  Temp2 = (byte)InString[12] - 48;
  Second = Temp1 * 10 + Temp2;
}
