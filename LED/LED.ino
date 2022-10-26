using namespace machinecontrol;

#include <Wire.h>
#include <Arduino_MachineControl.h>

int puls=DIN_READ_CH_PIN_00;
void setup() 
{
  Wire.begin();
  Serial.begin(9600);

  if (!digital_inputs.init()) {
    Serial.println("Digital input GPIO expander initialization fail!!");
  }
}

void loop() 
{

  Serial.println("Hello World");
}
