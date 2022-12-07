#include <Arduino_MachineControl.h>
#include <Wire.h>

using namespace machinecontrol;

//DICHIARAZIONI VARIABILI

//INGRESSI
int encoder1NastroA=DIN_READ_CH_PIN_00;
int ftc1NastroA=DIN_READ_CH_PIN_01;
int ftc2NastroB=DIN_READ_CH_PIN_02;
int encoder2NastroB=DIN_READ_CH_PIN_03;
int ftc3NastroB=DIN_READ_CH_PIN_04;
int ftc4NastroB=DIN_READ_CH_PIN_05;

//USCITE
int motore1NastroA=0;
int motore2NastroB=1;
int compressoreNastroB=2;
int pistoneBiancoNastroB=3;
int pistoneRossoNastroB=4;
int pistoneBluNastroB=5;

//VARIABILI
float res_divder;
float reference;
float raw_voltage_ch0;
float voltage_ch0;
float valMinimo;
bool abilitazione;
bool letturaFtc2NastroA;//Questa è una variabile globale

void setup() 
{

  //INIZIALIZZAZIONE VARIABILI
  res_divider = 1.5;
  reference = 3.3;
  raw_voltage_ch0=0;
  voltage_ch0=0;
  valMinimo=100;
  abilitazione=false;
  letturaFtc2NastroA=false;

  analogReadResolution(16);
  analog_in.set0_10V();

  Serial.begin(9600);
  Wire.begin();
  
  digital_outputs.setAll(0);
  if (!digital_inputs.init()) 
  {
    Serial.println("Digital input GPIO expander initialization fail!!");
  }
}

void loop() 
{
  NastroA();
}

void NastroA()
{
  //TUTTE LE FUNZIONI NASTROA 
  MarciaNastroA();
}

void NastroB()
{
  //TUTTE LE FUNZIONE NASTROB
}

void MarciaNastroA()
{
  //VARIABILI CHE SERVONO ALLA FUNZIONE
  bool letturaFtc1NastroA=digital_inputs.read(ftc1NastroA);
  letturaFtc2NastroA=digital_inputs.read(ftc2NastroA);

  if(letturaFtc1NastroA)
  {
    delay(1000);
    digital_outputs.set(motore1NastroA,HIGH);
  }
  else
  {
    Serial.println("Inserire Pezzo in Ingresso");
  }

  if(letturaFtc2NastroB)
  {
    digital_outputs.set(motore2NastroB,HIGH);
  }
  else
  {
    Serial.println("Il Pezzo non è uscito")
  }
}

void MarciaNastroB()
{
  //VARIABILI CHE SERVONO ALLA FUNZIONE
  bool letturaFtc3NastroB=digital_inputs.read(ftc3NastroB);

  if(letturaFtc3NastroB == 1 && letturaFtc2NastroA==0)
  {
    digital_outputs.set(motore1NastroA,LOW);
  }
  else
  {
    Serial.println("Il Pezzo non è ancora arrivato al NastroB");
  }
}
