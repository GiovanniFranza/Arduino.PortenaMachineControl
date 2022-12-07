#include <Arduino_MachineControl.h>
#include <Wire.h>
//#define LIMIT 3
#define LIMIT 10
using namespace machinecontrol;

float res_divider = 1.5;
float reference = 3.3;

int pippo=0;
int motore=0;
int compressore=1;
int pistoneBianco=2;
int pistoneRosso=3;
int pistoneBlu=4;
float valMinimo;
bool abilitazione;
int valore;
float raw_voltage_ch0;
float voltage_ch0;
int deleted;
//int queue[LIMIT];
int indice = 0;      // Indice che mi indica da dove inserire
int vettore[LIMIT];
void setup() 
{
  raw_voltage_ch0=0;
  voltage_ch0=0;
  valMinimo=100;

  reference=3.3;
  res_divider=1.5;
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

void loop() 
{
  AzionamentoMotore();
  AzionamentoCompressore();

  raw_voltage_ch0 = analog_in.read(0);
  voltage_ch0 = (raw_voltage_ch0 * reference) / 65535 / res_divider;
  valore=CheckColore(voltage_ch0);
  if(indice != LIMIT)
  {
    if (valore!=0)
    {
      vettore[indice] = valore;
      indice++;
    }
  }

  if(valore!=0)
  {
    Serial.println("");
    for(int i=0; i<LIMIT;i++)
    {
      Serial.print(vettore[i]);
    }
  }
}


int dequeue() 
{
    if(indice == 0)
    return -1;               // Underflow

    deleted = vettore[0];

      // Cambio gli indici degli elementi
    for(int i = 0; i < indice-1; i++)
        vettore[i] = vettore[i+1];

    indice--;
    return deleted;
}

void AzionamentoMotore()
{
  digital_outputs.set(motore,HIGH);
}

void AzionamentoCompressore()
{
  digital_outputs.set(compressore,HIGH);
}

void Espulsione(int valore)
{
  Serial.println(valore);
  switch (valore)
  {
    case 1:
      delay(50);
      digital_outputs.set(pistoneRosso,HIGH);
      delay(500);
      digital_outputs.set(pistoneRosso,LOW);
      break;
      
    case 2:
      delay(50);
      digital_outputs.set(pistoneBianco,HIGH);
      delay(500);
      digital_outputs.set(pistoneBianco,LOW);
      break;
      
    case 3:
      delay(50);
      digital_outputs.set(pistoneBlu,HIGH);
      delay(500);
      digital_outputs.set(pistoneBlu,LOW);
      break;
  }
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
    if(valMinimo>=1.25 && valMinimo<=1.40) 
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


/*
void printQueue() 
{
    if (tail == 0)
        Serial.println("Coda vuota!");

    else{
        Serial.print("\t(exit) ← ");
        for (int i = 0; i <= tail-1; i++)
            Serial.print(queue[ i ]);

        //Serial.println("Coda piena!\n\n");
        Serial.println("← (entry)");
    }
}

bool enqueue(int nuovoElemento) 
{
  if (tail == LIMIT)
    return false;               // Overflow

  queue[tail] = nuovoElemento;
  tail++;
  return true;
}


int dequeue() 
{
  if(tail == 0)
  return -1;               // Underflow

  deleted = queue[0];

    // Cambio gli indici degli elementi
  for(int i = 0; i < tail-1; i++)
      queue[i] = queue[i+1];

  tail--;
  return deleted;
}
*/