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
bool abilitazionePistoneBianco=false;
bool prev_tasto;
int valore;

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
  valore=0;
  
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
  int letturaFtc2=digital_inputs.read(ftc2);

  digital_outputs.set(motore,HIGH);
  float raw_voltage_ch0 = analog_in.read(0);
  float voltage_ch0 = (raw_voltage_ch0 * reference) / 65535 / res_divider;
  delay(100);
  
  valore=CheckColori(voltage_ch0);

  if(valore==1)
  {
    digital_outputs.set(pistoneBianco,HIGH);
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