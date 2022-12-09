#include <Arduino_MachineControl.h>
#include <Wire.h>

using namespace machinecontrol;

//DICHIARAZIONI VARIABILI

//INGRESSI
int encoder1NastroA=DIN_READ_CH_PIN_00;
int ftc2NastroA=DIN_READ_CH_PIN_01;
int ftc1NastroA=DIN_READ_CH_PIN_02;
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
bool valorePrecedenteFtc1NastroA;
bool valorePrecedenteFtc2NastroA;
bool valorePrecedenteEncoder2NastroB;
bool valorePrecedenteFtc4NastroB;
int count;
int faseNastroB;
bool pezzoOutSensorColor;
bool pezzoProntoEsplusione;

void setup() 
{

  //INIZIALIZZAZIONE VARIABILI
  res_divider = 1.5;
  reference = 3.3;
  raw_voltage_ch0=0;
  voltage_ch0=0;
  valMinimo=100;
  abilitazione=false;
  letturaFtc2NastroA;
  count=0;
  faseNastroB=1;

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
  digital_outputs.set(compressoreNastroB,HIGH);
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

  switch(faseNastroB)
  {
    case 1:
      raw_voltage_ch0 = analog_in.read(0);
      voltage_ch0 = (raw_voltage_ch0 * reference) / 65535 / res_divider;
      valoreLettoColore=LetturaColoreNastroB(voltage_ch0);
      Serial.println(valoreLettoColore);
      if(valoreLettoColore!=0)
      {
        pezzoOutSensorColor=true;
        faseNastroB=2;
      }
      break;
    case 2:
      if(pezzoOutSensorColor==true)
      {
        //pezzoProntoEsplusione=PresenzaPezzo();
        faseNastroB=3;
      }
      else
      {
        faseNastroB=1;
      }
      break;    
    case 3:
      if(PresenzaPezzo())
      {
        pezzoProntoEsplusione=true;
        pezzoOutSensorColor=false;
        faseNastroB=4;
      }
      else
      {
        faseNastroB=1;
      }
      break;
    case 4:
        if(pezzoProntoEsplusione==true)
        {
          switch(valoreLettoColore)
          {
            case 1:
              if(Conteggio())
              {
                if(count==3)
                {
                  faseNastroB=5;
                }
              }
              break;
            case 2:
              if(Conteggio())
              {
                if(count==9)
                {
                  faseNastroB=5;
                }
              }
              break;
            case 3:
              if(Conteggio())
              {
                if(count==15)
                {
                  faseNastroB=5;
                }
              }
            break;
          default:
            faseNastroB=1;
          }
        }
        else
        {
          pezzoProntoEsplusione=false;
          faseNastroB=1;
        }
      break;
    case 5:
      Espulsione(valoreLettoColore);
        pezzoProntoEsplusione=false;
        faseNastroB=1;
      break;
    default:
      faseNastroB=1;
  }
}

void MarciaNastroA()
{
  //VARIABILI CHE SERVONO ALLA FUNZIONE
  bool letturaFtc1NastroA=!digital_inputs.read(ftc1NastroA);
  letturaFtc2NastroA=!digital_inputs.read(ftc2NastroA);

  if(letturaFtc1NastroA && !valorePrecedenteFtc1NastroA)
  {
    delay(1000);//fare il fronte
    digital_outputs.set(motore1NastroA,HIGH);
  }

  valorePrecedenteFtc1NastroA=letturaFtc1NastroA;

  if(letturaFtc2NastroA && !valorePrecedenteFtc2NastroA)
  {
    digital_outputs.set(motore2NastroB,HIGH);
  }

  valorePrecedenteFtc2NastroA=letturaFtc2NastroA;
}

void MarciaNastroB()
{
  //VARIABILI CHE SERVONO ALLA FUNZIONE
  bool letturaFtc3NastroB=!digital_inputs.read(ftc3NastroB);

  if(letturaFtc3NastroB == 1)
  {
    digital_outputs.set(motore1NastroA,LOW);
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
      digital_outputs.set(pistoneRossoNastroB,HIGH);
      delay(500);
      digital_outputs.set(pistoneRossoNastroB,LOW);
      count=0;
      break;
      
    case 2:
      delay(50);
      digital_outputs.set(pistoneBiancoNastroB,HIGH);
      delay(500);
      digital_outputs.set(pistoneBiancoNastroB,LOW);
      count=0;
      break;
      
    case 3:
      delay(50);
      digital_outputs.set(pistoneBluNastroB,HIGH);
      delay(500);
      digital_outputs.set(pistoneBluNastroB,LOW);
      count=0;
      break;
  }
}

bool Conteggio()
{
  bool letturaEncoder2NastroB=digital_inputs.read(encoder2NastroB);

  if(letturaEncoder2NastroB && !valorePrecedenteEncoder2NastroB)
  {
    count++;
    return true;
  }
  else
  {
    //if(!letturaEncoder2NastroB && valorePrecedenteEncoder2NastroB)
    //{
    valorePrecedenteEncoder2NastroB=letturaEncoder2NastroB;
    return false;
    //}
  }
  valorePrecedenteEncoder2NastroB=letturaEncoder2NastroB;
}

bool PresenzaPezzo()
{
  bool valorePrecedenteFtc4NastroB=!digital_inputs.read(ftc4NastroB);

  if(ftc4NastroB && !valorePrecedenteFtc4NastroB)
  {
    return true;
  }
  else
  {
    //if(!ftc4NastroB && valorePrecedenteFtc4NastroB)
    //{
    valorePrecedenteFtc4NastroB=ftc4NastroB;
    return false;
    //}
  }
  valorePrecedenteFtc4NastroB=ftc4NastroB;
}