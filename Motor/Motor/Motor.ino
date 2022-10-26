#include <Wire.h>
#include <Arduino_MachineControl.h>

using namespace machinecontrol;

int pulsAvvio=DIN_READ_CH_PIN_00;
int ftcAvanti=DIN_READ_CH_PIN_01;
int ftcIndietro=DIN_READ_CH_PIN_02;
int motoreAvanti=0;
int motoreIndietro=0;


void setup() 
{
  Wire.begin();
  Serial.begin(9600);

  digital_outputs.setAll(0);

  digital_outputs.setAll(motoreAvanti,LOW);
  digital_outputs.setAll(motoreIndietro,LOW);

  if (!digital_inputs.init()) 
  {
    Serial.println("Digital input GPIO expander initialization fail!!");
  }
}

void loop() 
{
  Marcia
}

void Marcia()
{
  int valorePuls=digital_inputs_read(pulsAvvio);
  if(valorePuls==1 && ftcAvanti!=1)
  {
    digital_outputs.set(motoreAvanti,HIGH);
  }
}