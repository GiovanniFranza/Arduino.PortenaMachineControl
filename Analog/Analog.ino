#include <Arduino_MachineControl.h>
#include <Wire.h>
using namespace machinecontrol;

//float res_divider = 0.28057;
float res_divider = 1.5;
float reference = 3.3;
int motore=0;
int ledRosso=1;
int ledVerde=2;
int ledGiallo=3;
int encoder=DIN_READ_CH_PIN_00;
int contatore=0;
int var1=0;
int var2=0;
int var3=0;

void setup()
{
  analogReadResolution(16);
  Serial.begin(9600);
  Wire.begin();
  analog_in.set0_10V();
  digital_outputs.setAll(0);
  digital_outputs.set(motore,LOW);
  digital_outputs.set(ledRosso,LOW);
  digital_outputs.set(ledVerde,LOW);
  digital_outputs.set(ledGiallo,LOW);

  if (!digital_inputs.init()) 
  {
    Serial.println("Digital input GPIO expander initialization fail!!");
  }
}

void loop() 
{
  bool abilitazione=false;
  float sogliaRiferimento=1.49;
  int valMinimo=100;

  int letturaEncoder=digital_inputs.read(encoder);
  digital_outputs.set(motore,HIGH);
  float raw_voltage_ch0 = analog_in.read(0);
  float voltage_ch0 = (raw_voltage_ch0 * reference) / 65535 / res_divider;
  delay(100);
  
  if(voltage_ch0 < sogliaRiferimento && sogliaRiferimento >=1.49 && sogliaRiferimento<=1.50)
  {
    abilitazione=true;
  }else
  {
    abilitazione=false;
  }

  if(abilitazione)
  {
    if(voltage_ch0!=valMinimo)
    {
      valMinimo=voltage_ch0;
    }
  }

  if(valMinimo!=100 & abilitazione==true)
  {
    if(voltage_ch0>=1.25 && voltage_ch0<=1.30) 
    {
      var1=1;
    }
    if(var1==1)
    {
      Serial.println("Rosso");
      delay(1000);
      Serial.println("------");
      var1=0;
    }
    else
    {    
      Serial.println("------");
    }
    if(voltage_ch0>=0.7 && voltage_ch0<=0.8)
    {
      var2=1;
    }
    if(var2==1)
    {
      Serial.println("Bianco");
      delay(1000);
      Serial.println("------");
      var2=0;
    }
    else
    {    
      Serial.println("------");
    }

    if(voltage_ch0>=1.38 && voltage_ch0<=1.42)
    {
      var3=1;
    }
    if(var3==1)
    {
      Serial.println("Blu");
      delay(1000);
      Serial.println("------");
      var3=0;
    }
    else
    {    
      Serial.println("------");
    }
  }
  valMinimo=100;
}