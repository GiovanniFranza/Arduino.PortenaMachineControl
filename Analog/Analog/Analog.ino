#include <Arduino_MachineControl.h>
using namespace machinecontrol;

//float res_divider = 0.28057;
float res_divider = 1.5;
float reference = 3.3;
int motore=0;
void setup()
{
  analogReadResolution(16);

  Serial.begin(9600);
  analog_in.set0_10V();
  digital_outputs.setAll(0);
  digital_outputs.set(motore,LOW);
}

void loop() 
{
  
    digital_outputs.set(motore,HIGH);
  float raw_voltage_ch0 = analog_in.read(0);
  float voltage_ch0 = (raw_voltage_ch0 * reference) / 65535 / res_divider;
  Serial.println(voltage_ch0);
  delay(100);
}
