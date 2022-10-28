#include <Wire.h>
#include <Arduino_MachineControl.h>

using namespace machinecontrol;

int pulsAvvio=DIN_READ_CH_PIN_00;
int ftcAvanti=DIN_READ_CH_PIN_01;
int ftcIndietro=DIN_READ_CH_PIN_02;
int motoreAvanti=0;
int motoreIndietro=1;
int statoAvanti=0;
int statoIndietro=0;    
int statoPulsante= 0; 


void setup() 
{
  Wire.begin();
  Serial.begin(9600);

  digital_outputs.setAll(0);

  digital_outputs.set(motoreAvanti,LOW);
  digital_outputs.set(motoreIndietro,LOW);

  if (!digital_inputs.init()) 
  {
    Serial.println("Digital input GPIO expander initialization fail!!");
  }
}

void loop() 
{
  Marcia();
}

void Marcia()
{
  int valorePuls=digital_inputs.read(pulsAvvio);
  int valoreFotAvanti=digital_inputs.read(ftcAvanti);
  int valoreFotIndietro=digital_inputs.read(ftcIndietro);
  int controllo=0;

  if(valorePuls != statoPulsante)
  { 
    if(valorePuls == 1 && controllo==0)
    {
      if(statoAvanti==0)
        digital_outputs.set(motoreAvanti,HIGH);
        statoAvanti=1;
    }
  }

  if(statoAvanti==1 && valoreFotAvanti==0)
  {
    digital_outputs.set(motoreAvanti,LOW);
    delay(1000);
    digital_outputs.set(motoreIndietro,HIGH);
    controllo=1;
  }

  if(statoAvanti==1 && valoreFotIndietro==0)
  {
    digital_outputs.set(motoreIndietro,LOW);
    delay(1000);
    digital_outputs.set(motoreAvanti,HIGH);
    controllo=1;
  }
}
