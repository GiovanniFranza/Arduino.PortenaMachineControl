const int PULSANTE=2;
const int LED_PEDONE_GIALLO=9;
const int LED_PEDONE_VERDE=10;
const int LED_SEMAFORO_ROSSO=11;
const int LED_SEMAFORO_GIALLO=12;
const int LED_SEMAFORO_VERDE=13;
bool prev_tasto=false;
volatile bool stato = false;

void setup()
{
  pinMode(PULSANTE, INPUT);
  attachInterrupt(digitalPinToInterrupt(PULSANTE), cambia, FALLING);
    
  pinMode(LED_PEDONE_GIALLO,OUTPUT);  
  pinMode(LED_PEDONE_VERDE,OUTPUT);

  pinMode(LED_SEMAFORO_ROSSO,OUTPUT); 
  pinMode(LED_SEMAFORO_GIALLO,OUTPUT);
  pinMode(LED_SEMAFORO_VERDE,OUTPUT);

  digitalWrite(LED_PEDONE_GIALLO,LOW);  
  digitalWrite(LED_PEDONE_VERDE,LOW);
    
  digitalWrite(LED_SEMAFORO_ROSSO,LOW); 
  digitalWrite(LED_SEMAFORO_GIALLO,LOW);
  digitalWrite(LED_SEMAFORO_VERDE,LOW);
}
void loop()
{
  int risultato=Semaforo();
  Pedone(risultato,stato);
} 


int Semaforo()
{
  if(stato==0)
  {
  	bool consenso;
  	consenso=0;
  	digitalWrite(LED_SEMAFORO_VERDE,HIGH);
  	delay(5000);
  	digitalWrite(LED_SEMAFORO_VERDE,LOW);
  	digitalWrite(LED_SEMAFORO_GIALLO,HIGH);
  	delay(1500);
  	digitalWrite(LED_SEMAFORO_ROSSO,HIGH);
  	consenso=1;
  	digitalWrite(LED_SEMAFORO_GIALLO,LOW);
  	delay(1000);
  	digitalWrite(LED_SEMAFORO_ROSSO,LOW);
  	delay(500);
  	return consenso;
  }
}

void Pedone(int prenotazione,bool tasto)
{
  if (!prev_tasto && tasto && prenotazione==1) 
  {
   digitalWrite(LED_PEDONE_GIALLO,HIGH);
   digitalWrite(LED_SEMAFORO_ROSSO,HIGH);
   delay(2000);
   digitalWrite(LED_PEDONE_VERDE,HIGH);
   delay(1000);
   digitalWrite(LED_PEDONE_GIALLO,LOW);
   digitalWrite(LED_PEDONE_VERDE,LOW); 
   digitalWrite(LED_SEMAFORO_ROSSO,LOW);
   prev_tasto = !tasto;
   //devo per forza cambiare segno a prev_tasto altrimenti non funziona
   //diversamente dall'esercizio di aliverti sul fronte, dove il cambio
   //avveniva al premere o al rilascio del pulsante
   stato=!stato;
   //serve per poter entrare di nuovo nel ciclo del semaforo 
  }
}

void cambia() 
{
  stato = !stato;
}
