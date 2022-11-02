#include <Arduino_MachineControl.h>
using namespace machinecontrol;

//float res_divider = 0.28057;
float res_divider = 1.5;
float reference = 3.3;
int motore=0;
int ledRosso=1;
int ledVerde=2;
int ledGiallo=3;
void setup()
{
  analogReadResolution(16);

  Serial.begin(9600);
  analog_in.set0_10V();
  digital_outputs.setAll(0);
  digital_outputs.set(motore,LOW);
  digital_outputs.set(ledRosso,LOW);
  digital_outputs.set(ledVerde,LOW);
  digital_outputs.set(ledGiallo,LOW);
}

void loop() 
{
  
  digital_outputs.set(motore,HIGH);
  float raw_voltage_ch0 = analog_in.read(0);
  float voltage_ch0 = (raw_voltage_ch0 * reference) / 65535 / res_divider;
  Serial.println(voltage_ch0);
  delay(100);

  if(voltage_ch0>=1.25&&voltage_ch0<=1.30)
  {
  digital_outputs.set(ledRosso,HIGH);
  }

  if(voltage_ch0>=0.7&&voltage_ch0<=0.8)
  {
  digital_outputs.set(ledVerde,HIGH);
  }

  if(voltage_ch0>=1.38&&voltage_ch0<=1.40)
  {
  digital_outputs.set(ledGiallo,HIGH);
  }
  
}
