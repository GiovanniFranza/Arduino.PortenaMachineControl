
#include <Wire.h>
#include <Arduino_MachineControl.h>
using namespace machinecontrol;

int puls=DIN_READ_CH_PIN_00;
int led=0;
void setup() 
{
  Wire.begin();
  Serial.begin(9600);
  digital_outputs.setAll(0);

  if (!digital_inputs.init()) {
    Serial.println("Digital input GPIO expander initialization fail!!");
  }
}

void loop() 
{
  if(digital_inputs.read(puls)==1)
  {
    digital_outputs.set(led,HIGH);
  }
  else
  {
    digital_outputs.set(led,LOW);

  }
}
