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
int pistone=2;
int statoMacchina;
bool pezzoIn;
bool abilitazione;

void setup() 
{
  
  statoMacchina=1; 
  prev_Encoder=1;
  prev_FtcExit=1;
  prev_Contatore=0;
  count=0;
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
  float raw_voltage_ch0 = analog_in.read(0);
  float voltage_ch0 = (raw_voltage_ch0 * reference) / 65535 / res_divider;

  switch(statoMacchina)
  {
    case 1:
      AzionamentoMotore();
      AzionamentoCompressore();
      statoMacchina=2;
    break;
    case 2:
      if(CheckFronteExitEspulsione())
      {
        pezzoIn=true;
        statoMacchina=3;
      }
    break;
    case 3:
      if(pezzoIn==true)
      {
        if(CheckFronteEncoder())
        {
          if(count==1)
          {
            statoMacchina=4;
          }
        }
      }
    break;
    case 4:
      Espulsione();
      statoMacchina=1;
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

void Espulsione()
{
    digital_outputs.set(pistone,HIGH);
    Serial.println("PistoneFuori");
    delay(100);
    digital_outputs.set(pistone,LOW);
    Serial.println("PistoneDentro");
    count=0;
}

void Contatore()
{
    count++;
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


bool CheckFronteExitEspulsione()
{
  Serial.println("AAA");
  int letturaFtcExit=digital_inputs.read(ftcEspulsione);
  if((letturaFtcExit !=prev_FtcExit) && letturaFtcExit==HIGH)
  {
    Serial.println("FronteAttivoFtcExit");
    prev_FtcExit=letturaFtcExit;
    return true;
  }
  else
  {
    Serial.println("FronteNonAttivoFtcExit");
    prev_FtcExit=letturaFtcExit;
    return false;
  }
}

bool CheckFronteEncoder()
{
  int letturaEncoder=digital_inputs.read(encoder);
  if ((letturaEncoder != prev_Encoder) && letturaEncoder == HIGH)
  {
    Serial.println("FronteAttivoEncoder");
    prev_Encoder=letturaEncoder;
    Contatore();
    return true;
  }
  else
  {
    Serial.println("FronteNonAttivoEncoder");
    prev_Encoder=letturaEncoder;
    return false;
  }
}