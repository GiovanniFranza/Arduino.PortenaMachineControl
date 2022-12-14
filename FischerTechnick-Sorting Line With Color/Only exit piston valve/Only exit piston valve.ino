
#include <Arduino_MachineControl.h>
#include <Wire.h>
using namespace machinecontrol;

float res_divider;
float reference;
int encoder=DIN_READ_CH_PIN_00;
int ftcEspulsione=DIN_READ_CH_PIN_01;
int prev_Encoder;
int prev_FtcExit;
int prev_Contatore;
int count;
int motore=0;
int compressore=1;
int pistoneBianco=2;
int pistoneRosso=3;
int pistoneBlu=4;
int statoMacchina;
bool pezzoIn;
int conteggio;
float valMinimo;
bool abilitazione;
int valore;
float raw_voltage_ch0;
float voltage_ch0;

void setup() 
{ 
  raw_voltage_ch0=0;
  voltage_ch0=0;
  valMinimo=100;
  statoMacchina=1; 
  prev_Encoder=1;
  prev_FtcExit=1;
  prev_Contatore=0;
  count=0;
  valore=0;

  reference=3.3;
  res_divider=1.5;
  abilitazione=false;

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
  switch(statoMacchina)
  {
    case 1:
      AzionamentoMotore();
      AzionamentoCompressore();
      statoMacchina=2;
      break;
    case 2:
      raw_voltage_ch0 = analog_in.read(0);
      voltage_ch0 = (raw_voltage_ch0 * reference) / 65535 / res_divider;
      valore=CheckColore(voltage_ch0);
      if(valore!=0)
      {
        statoMacchina=3;
      }
      break;
    case 3:
      if(CheckFronteExitEspulsione())
        {
          pezzoIn=true;
          statoMacchina=4;
        }
      break;
    case 4:
      if(pezzoIn==true)
      {
        statoMacchina=5;
      }
      break;
    case 5:
      if(pezzoIn==true)
      {
        switch(valore)
        {
          case 1:
            if(CheckFronteEncoder())
            {
              if(count==9)
              {
                statoMacchina=6;
              }
            }
            break;
          case 2:
            if(CheckFronteEncoder())
            {
              if(count==3)
              {
                statoMacchina=6;
              }
            }
            break;
          case 3:
            if(CheckFronteEncoder())
            {
              if(count==15)
              {
                statoMacchina=6;
              }
            }
            break;
          default:
            statoMacchina=2;          
        }
      }
      else
      {
        statoMacchina=2;
      }
      break;
    case 6:
      Espulsione(valore);
      statoMacchina=2;
    break;
  }
}

void AzionamentoMotore()
{
  digital_outputs.set(motore,HIGH);
}

void AzionamentoCompressore()
{
  digital_outputs.set(compressore,HIGH);
}

void Espulsione(int valore)
{
  switch (valore)
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

bool CheckFronteExitEspulsione()
{
  int letturaFtcExit=digital_inputs.read(ftcEspulsione);
  if((letturaFtcExit !=prev_FtcExit) && letturaFtcExit==LOW)
  {
    prev_FtcExit=letturaFtcExit;
    return true;
  }
  else
  {
    prev_FtcExit=letturaFtcExit;
    return false;
  }
}

bool CheckFronteEncoder()
{
  int letturaEncoder=digital_inputs.read(encoder);
  if ((letturaEncoder != prev_Encoder) && letturaEncoder == HIGH)
  {
    prev_Encoder=letturaEncoder;
    count++;
    return true;
  }
  else
  {
    prev_Encoder=letturaEncoder;
    return false;
  }
}

int CheckColore(float valoreAnalogico)
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
    if(valMinimo>=1.25 && valMinimo<=1.30) 
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