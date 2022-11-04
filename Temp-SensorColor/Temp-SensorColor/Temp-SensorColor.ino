#include <Arduino_MachineControl.h>
#include <Wire.h>
using namespace machinecontrol;

float res_divider = 1.5;
float reference = 3.3;
int motore=0;
int encoder=DIN_READ_CH_PIN_00;
int contatore=0;
int conteggio;
const bool ON=HIGH;
const bool OFF=LOW;
long t0;//instante iniziale
long t;

void setup() 
{
  conteggio=OFF;

  analogReadResolution(16);
  Serial.begin(9600);
  Wire.begin();
  analog_in.set0_10V();
  digital_outputs.setAll(0);

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
  Serial.println(voltage_ch0);
  delay(100);

  if((raw_voltage_ch0==HIGH) && (conteggio==OFF))
  {
    conteggio=ON;
    t0=mills();
    Serial.println(Inizio);
  }

  if((raw_voltage_ch0==LOW) && (conteggio==ON))
  {
    conteggio=OFF;
    t=mills()-t0;
    Serial.println(t);
  }

}
