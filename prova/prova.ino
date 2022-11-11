#include <Arduino_MachineControl.h>
#include <Wire.h>
using namespace machinecontrol;

int encoder=DIN_READ_CH_PIN_00;
int ftcEspulsione=DIN_READ_CH_PIN_01;
int prev_Encoder;
int prev_FtcExit;
int count;
int motore=0;
int compressore=1;
int pistone=2;
int statoMacchina;
void setup() 
{ 
  statoMacchina=0; 
  prev_Encoder=0;
  prev_FtcExit=0;
  count=0;

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
    bool pezzoIn;
    bool pezzOut;
    int conteggio;

    case 0:
      AzionamentoMotore();
      AzionamentoCompressore();
      statoMacchina=1;
      break;
    case 1:
      pezzoIn=CheckFronteExitEspulsione();
      statoMacchina=2;
      break;
    case 2:
      if(pezzoIn==true)
      {
        pezzOut=CheckFronteEncoder();
        statoMacchina=3;
      }
      break;
    case 3:
      if(pezzOut==true)
      {
        conteggio=Contatore();
        statoMacchina=4;
      }
      break;
    case 4:
      Espulsione(conteggio);
      statoMacchina=0;
      break;
  }
  /*
  if(CheckFronteExitEspulsione())
  {
    if(CheckFronteEncoder())
    {
      int var1=Contatore();
      if(var1==5)
      {
        digital_outputs.set(pistone,HIGH);
        delay(500);
        digital_outputs.set(pistone,LOW);
        count=0;
      }
    //}
  }
  */
}

void AzionamentoMotore()
{
  digital_outputs.set(motore,HIGH);
}

void AzionamentoCompressore()
{
  digital_outputs.set(compressore,HIGH);
}

void Espulsione(int conteggio)
{
  if(conteggio==5)
  {
    digital_outputs.set(pistone,HIGH);
    delay(500);
    digital_outputs.set(pistone,LOW);
    count=0;
  }
}

int Contatore()
{
    count++;
    Serial.println(count);
    return count;
}

bool CheckFronteExitEspulsione()
{
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
    //count++;
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