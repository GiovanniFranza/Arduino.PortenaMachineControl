#include <Arduino_MachineControl.h>
#include <Wire.h>
using namespace machinecontrol;

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
bool pezzOut;
int conteggio;

void setup() 
{ 
  statoMacchina=1; 
  prev_Encoder=1;
  prev_FtcExit=1;
  prev_Contatore=0;
  count=0;

  Serial.begin(9600);
  delay(10);
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
      if(CheckFronteExitEspulsione())
      {
        pezzoIn=true;
        statoMacchina=3;
      }
    break;
    //case 3:
      //if(pezzoIn==true)
      //{
        //if(CheckFronteEncoder())
        //{
          //pezzOut=true;
          //statoMacchina=4;
        //}
      //}
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
        //conteggio=Contatore(pezzOut);
        //statoMacchina=5;
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
  //if(conteggio==5)
  //{
    digital_outputs.set(pistone,HIGH);
    Serial.println("PistoneFuori");
    delay(500);
    digital_outputs.set(pistone,LOW);
    Serial.println("PistoneDentro");

    count=0;
  //}
}

int Contatore(bool var)
{
  if(var==HIGH)
  {
    count++;
    Serial.println(count);
    return count;
  }
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
    count++;
    //Serial.println(count);
    return true;
  }
  else
  {
    Serial.println("FronteNonAttivoEncoder");
    prev_Encoder=letturaEncoder;
    return false;
  }
}