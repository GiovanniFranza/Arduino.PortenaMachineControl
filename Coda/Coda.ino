#include <Arduino_MachineControl.h>
#include <Wire.h>
#include <Arduino.h>
#define MAX 3
using namespace machinecontrol;

int encoder2NastroB=DIN_READ_CH_PIN_03;
int ftc3NastroB=DIN_READ_CH_PIN_04;
int ftc4NastroB=DIN_READ_CH_PIN_05;
int vettore[MAX];
int motore2NastroB=1;
int compressoreNastroB=2;
int pistoneBiancoNastroB=3;
int pistoneRossoNastroB=4;
int pistoneBluNastroB=5;
bool letturaFtc2NastroA;
int valoreLettoColore;
bool valorePrecedenteFtc4NastroB;
int count;
bool valorePrecedenteEncoder2NastroB;
int faseNastroB;
bool pezzoOutSensorColor;
bool pezzoProntoEsplusione;
float res_divider;
float reference;
float raw_voltage_ch0;
float voltage_ch0;
float valMinimo;
bool abilitazione;

void setup() 
{
  //INIZIALIZZAZIONE VARIABILI
  valorePrecedenteEncoder2NastroB=1;
  valorePrecedenteFtc4NastroB=1;
  res_divider = 1.5;
  reference = 3.3;
  raw_voltage_ch0=0;
  voltage_ch0=0;
  valMinimo=100;
  abilitazione=false;
  letturaFtc2NastroA;
  count=0;
  faseNastroB=1;
  valoreLettoColore=0;
  
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
  NastroB();

  raw_voltage_ch0 = analog_in.read(0);
  voltage_ch0 = (raw_voltage_ch0 * reference) / 65535 / res_divider;
  valoreLettoColore=LetturaColoreNastroB(voltage_ch0);

  if(valoreLettoColore!=0)
  {
    for(int i=0; i<MAX; i++)
    {
      vettore[i]=valoreLettoColore;
      count++;
    }
  }

  if(count==MAX)
  {
    for (int i=0; i<MAX; i++)
    {
      Serial.println(vettore[i]);
    }
  }  
}

void NastroB()
{
  digital_outputs.set(motore2NastroB, HIGH);
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
