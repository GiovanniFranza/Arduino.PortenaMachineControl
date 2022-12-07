#include <Arduino_MachineControl.h>
#include <Wire.h>

using namespace machinecontrol;

//DICHIARAZIONI VARIABILI

//INGRESSI
int encoder1NastroA=DIN_READ_CH_PIN_00;
int ftc1NastroA=DIN_READ_CH_PIN_01;
int ftc2NastroB=DIN_READ_CH_PIN_02;
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
float res_divder;
float reference;
float raw_voltage_ch0;
float voltage_ch0;
float valMinimo;
bool abilitazione;

void setup() 
{

  //INIZIALIZZAZIONE VARIABILI
  res_divider = 1.5;
  reference = 3.3;
  raw_voltage_ch0=0;
  voltage_ch0=0;
  valMinimo=100;
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

void loop() {
  // put your main code here, to run repeatedly:

}
