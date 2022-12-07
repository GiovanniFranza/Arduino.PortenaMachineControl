#include <Arduino_MachineControl.h>
#include <Wire.h>

using namespace machinecontrol;

//Enumerazione
enum valori {Bianco, Rosso, Blu};
enum valori valore;

//DICHIARAZIONI VARIABILI

//INGRESSI
int encoder1NastroA=DIN_READ_CH_PIN_00;
int ftc1NastroA=DIN_READ_CH_PIN_01;
int ftc2NastroA=DIN_READ_CH_PIN_02;
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
float res_divider;
float reference;
float raw_voltage_ch0;
float voltage_ch0;
float valMinimo;
bool abilitazione;
bool letturaFtc2NastroA;
int valoreLettoColore;

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
  AzionamentoCompressore();
  NastroA();
  NastroB();
}

void AzionamentoCompressore()
{
  digital_outputs.set(compressore,HIGH);
}

void NastroA()
{
  //TUTTE LE FUNZIONI NASTROA 
  MarciaNastroA();
}

void NastroB()
{
  //TUTTE LE FUNZIONE NASTROB
  MarciaNastroB();

  raw_voltage_ch0 = analog_in.read(0);
  voltage_ch0 = (raw_voltage_ch0 * reference) / 65535 / res_divider;
  valore=LetturaColoreNastroB(voltage_ch0);
  
  Espulsione(valore);
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

  if(letturaFtc2NastroA)
  {
    digital_outputs.set(motore2NastroB,HIGH);
  }
  else
  {
    Serial.println("Il Pezzo non è uscito");
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

int LetturaColoreNastroB(float valoreAnalogico)
{
  int valRitorno=0;
  if(!(valoreAnalogico >= 1.46 &&  valoreAnalogico <= 1.53))
  {
    abilitazione=true;
  }
  else
  {
    abilitazione=false;
  }

  if(abilitazione)
  {
    if(valoreAnalogico<=valMinimo)
    {
      valMinimo=valoreAnalogico;
    }
  }
  if(valMinimo!=100 & abilitazione==false)
  {
    if(valMinimo>=1.25 && valMinimo<=1.40) 
    {
      valRitorno=1;//Rosso
    }
    if(valMinimo>=0.7 && valMinimo<=0.8)
    {
      valRitorno=2; //Bianco
    }
    if(valMinimo>=1.38 && valMinimo<=1.42)
    {
      valRitorno=3;//Blue
    }
    valMinimo=100;
  }
  return valRitorno;
}

void Espulsione(int codiceColore)
{
  switch (codiceColore)
  {
    case 1:
      delay(50);
      digital_outputs.set(pistoneRosso,HIGH);
      delay(500);
      digital_outputs.set(pistoneRosso,LOW);
      count=0;
      break;
      
    case 2:
      delay(50);
      digital_outputs.set(pistoneBianco,HIGH);
      delay(500);
      digital_outputs.set(pistoneBianco,LOW);
      count=0;
      break;
      
    case 3:
      delay(50);
      digital_outputs.set(pistoneBlu,HIGH);
      delay(500);
      digital_outputs.set(pistoneBlu,LOW);
      count=0;
      break;
  }
}