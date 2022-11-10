#include <Arduino_MachineControl.h>
#include <Wire.h>
using namespace machinecontrol;

//float res_divider = 0.28057;
int statoMacchina;
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
int valore;
int prev_tasto;

void setup()
{
  valMinimo=100;
  sogliaRiferimento=1.49;
  abilitazione=false;
  motore=0;//OUT
  compressore=1;//OUT
  pistoneBianco=2;//OUT
  reference=3.3;
  res_divider=1.5;
  contatore=0;
  valore=0;
  prev_tasto=1;
  statoMacchina=1;

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
  float raw_voltage_ch0 = analog_in.read(0);
  float voltage_ch0 = (raw_voltage_ch0 * reference) / 65535 / res_divider;
  delay(100);
  switch(statoMacchina)
  {
    case 1:
      AzionaMotore();
      statoMacchina=2;
      CheckFronte();
      break;
    case 2:
      valore=CheckColore(voltage_ch0);
      if(valore!=0)
        statoMacchina=3;
      break;
    case 3:
      if(valore==2 && CheckFronte()==true)
      {
        ExitPistoneBianco();
      }
      statoMacchina=1;
      break;
  }
}

void AzionaMotore()
{
  digital_outputs.set(motore,HIGH);
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

bool CheckFronte()
{
  int letturaFtc2=digital_inputs.read(ftc2);
  delay(50);
  if ((letturaFtc2 != prev_tasto) && letturaFtc2 == LOW)
  {
    Serial.println("FronteAttivo");
    prev_tasto=letturaFtc2;
    return true;
  }
  else
  {
    Serial.println("FronteNonAttivo");
    prev_tasto=letturaFtc2;
    return false;
  }
}

void ExitPistoneBianco()
{
  /*
  int letturaEncoder=digital_inputs.read(encoder);
  Serial.println(letturaEncoder);
  if(letturaEncoder==5)
  {
    digital_outputs.set(pistoneBianco,HIGH);
    delay(1000);
    digital_outputs.set(pistoneBianco,LOW);
    letturaEncoder=0;
  }
  */
  Serial.println("pippo");
}