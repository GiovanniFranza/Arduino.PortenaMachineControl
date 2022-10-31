#include <Wire.h>
#include <Arduino_MachineControl.h>

using namespace machinecontrol;

//Inizializzazione Variabili
int pulsAvvio=DIN_READ_CH_PIN_00;
int ftcAvanti=DIN_READ_CH_PIN_01;
int ftcIndietro=DIN_READ_CH_PIN_02;
int pulsStop=DIN_READ_CH_PIN_03;
int motoreAvanti=0;
int motoreIndietro=1;
int statoPulsanteStart= 0; 
int statoPulsanteStop= 1; 
int avvio=0;
int stop=0;

void setup() 
{
  Serial.begin(9600);
  Wire.begin();
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
  int valorePulsAvvio=digital_inputs.read(pulsAvvio);//Leggo il valore del pulsante Start
  int valorePulsStop=digital_inputs.read(pulsStop);//Leggo il valore del pulsante Stop
  int valoreFotAvanti=digital_inputs.read(ftcAvanti);//Leggo il valore della FTC1
  int valoreFotIndietro=digital_inputs.read(ftcIndietro);//Leggo il valore della FTC2
  int controllo=0;//Variabile di controllo 


//Esegue lo stop di entrambi i motori e rimette a 0 la variabile di avvio
  if(valorePulsStop != 1)
  {
    digital_outputs.set(motoreAvanti,LOW);
    digital_outputs.set(motoreIndietro,LOW);
    avvio=0;
  }


//Esegue la partenza del motore tramite il pulsante di start e mette in moto il motore in avanti
//Ed lo esegue una sola volta
  if(valorePulsAvvio != statoPulsanteStart)
  {
    Serial.println(valorePulsAvvio);
    if(valorePulsAvvio == 1 && controllo==0)
    {
      if(avvio==0)
      {
        digital_outputs.set(motoreAvanti,HIGH);
        avvio=1;
      }
    }
    Serial.println(statoPulsanteStart);
  }

  if(avvio==1 && valoreFotAvanti==0)
  {
    digital_outputs.set(motoreAvanti,LOW);
    delay(1000);
    digital_outputs.set(motoreIndietro,HIGH);
    controllo=1;
  }

  if(avvio==1 && valoreFotIndietro==0)
  {
    digital_outputs.set(motoreIndietro,LOW);
    delay(1000);
    digital_outputs.set(motoreAvanti,HIGH);
    controllo=1;
  }
}

