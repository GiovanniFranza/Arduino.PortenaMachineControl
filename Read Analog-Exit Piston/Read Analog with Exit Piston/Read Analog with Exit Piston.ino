#include <Arduino_MachineControl.h>
#include <Wire.h>
using namespace machinecontrol;

//float res_divider = 0.28057;

int contatore;
float res_divider;
float reference;
int motore;
int compressore;
int pistoneBianco;
int encoder=DIN_READ_CH_PIN_00;
int ftc2=DIN_READ_CH_PIN_01;
bool abilitazione;
float sogliaRiferimento;
float valMinimo;
const bool abilitazionePistoneBianco=false;

void setup()
{
  valMinimo=100;
  sogliaRiferimento=1.49;
  abilitazione=false;
  motore=0;
  compressore=1;
  pistoneBianco=2;
  reference=3.3;
  res_divider=1.5;
  contatore=0;
  analogReadResolution(16);
  Serial.begin(9600);
  Wire.begin();
  analog_in.set0_10V();
  digital_outputs.setAll(0);
  digital_outputs.set(motore,LOW);
  digital_outputs.set(compressore,HIGH);
  digital_outputs.set(pistoneBianco,LOW);

  if (!digital_inputs.init()) 
  {
    Serial.println("Digital input GPIO expander initialization fail!!");
  }
}

void loop() 
{
  int letturaEncoder=digital_inputs.read(encoder);

  digital_outputs.set(motore,HIGH);
  float raw_voltage_ch0 = analog_in.read(0);
  float voltage_ch0 = (raw_voltage_ch0 * reference) / 65535 / res_divider;
  delay(100);
  
  
  if(!(voltage_ch0 >= 1.46 &&  voltage_ch0 <= 1.53))
  {
    abilitazione=true;
  }
  else
  {
    abilitazione=false;
  }

  if(abilitazione)
  {
    if(voltage_ch0<=valMinimo)
    {
      valMinimo=voltage_ch0;
    }
  }
  if(valMinimo>=0.7 && valMinimo<=0.8)
  {
    Serial.println("Bianco");
    abilitazionePistoneBianco=true;
  }
  else
  {    
    Serial.println("------");
  }

  if(abilitazionePistone && ftc2==OFF)
  {
    digital_outputs.set(pistoneBianco,HIGH);
    delay(1000);
    digital_outputs.set(pistoneBianco,LOW);
  }
  else
  {
    digital_outputs.set(pistoneBianco,LOW);
  }

  valMinimo=100;
}