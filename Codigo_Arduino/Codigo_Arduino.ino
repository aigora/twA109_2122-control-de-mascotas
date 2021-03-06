#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define pinTrigger 10
#define pinEcho 9
OneWire ourWire(2); //Comunico el programa con el pin 2 del Arduino
DallasTemperature sensor(&ourWire);

int analogInPin = A0;
int analogValue = 0;
int led = 13;

int TRIG = 10; //Variable que contiene el número del pin al cual conectemos la señal "trigger"
int ECO = 9; //Variable que contiene el número del pin al cual conectamos la señal "echo"
int estado = 0;
int altura = 15; //Constante altura a la que se encuentra el sensor ultrasonido de la base de la piscina
int DURACION; //Variable duración para la función nivel_bebedero_consigna
float DISTANCIA; //Variable distancia para la función nivel _bebedero_consigna
Servo servo1 ;
float distancia; //Variable distancia para la función mostrar_nivel_bebedero, de tipo float que contendrá la distancia en cm
int numero2;
long tiempo; //Variable tiempo para la función mostrar_nivel_bebedero


void setup()
{
  delay(1000);
  Serial.begin(9600);

  sensor.begin();
  pinMode(led, OUTPUT);

  pinMode(pinTrigger, OUTPUT); //Configuramoms el pin de "trigger" como salida
  pinMode(pinEcho, INPUT);  //Configuramoms el pin de "echo" como entrada
  digitalWrite(pinTrigger, LOW);//Ponemos en voltaje bajo(0V) el pin de "trigger"


}

void loop()
{

  if (Serial.available() > 0)  // Si hay mensajes procedentes del PC
    procesar_mensaje();
  
  switch (estado) {
    case 0: break;
    case 1 :
      if (medir_distancia() > numero2) {
        servo1.write(0);
        estado = 0;
      }
  }


}
// Resto de acciones


float medir_distancia(void)
{
  float DISTANCIA;
  long DURACION;

  digitalWrite(TRIG, HIGH);
  delay(1);
  digitalWrite(TRIG, LOW);
  DURACION = pulseIn(ECO, HIGH);
  DISTANCIA = DURACION / 58.2;
  return (DISTANCIA);
}


void procesar_mensaje(void)
{
  int numero1;
  int numero2;
  int numero3;
  float temp;
  String cadena = Serial.readStringUntil('\n'); // Lee mensaje
  String valor = Serial.readStringUntil('\n');  // Lee valor
  numero1 = valor.toInt();  // Transforma valor a entero
  numero2 = valor.toInt();
  numero3 = valor.toInt();

  if (cadena.equals("MOSTRAR_TEMP")) // Entre las comillas se pone el texto del mensaje que se espera
  {
    sensor.requestTemperatures();
    temp = sensor.getTempCByIndex(0);
    temp = temp * 100;
    numero1 = int(temp);
    Serial.println(numero1);
  }
  else if (cadena.equals("ILUMINACION")) // Y así sucesivamente con todos los posibles mensajes
  {
    numero1 = digitalRead(led);
    Serial.println(numero1);

  }
  else if (cadena.equals("AUTOMATICO"))
  {
    analogValue = analogRead(analogInPin);
    Serial.println(analogValue);
    delay(10);
    if (analogValue < 800) {
      digitalWrite(led, HIGH);
    } else {
      digitalWrite(led, LOW);
    }

  }
  else if (cadena.equals("APAGA"))
  {
    digitalWrite(led, LOW);
  }
  else if (cadena.equals("ENCIENDE"))
  {
    digitalWrite(led, HIGH);
  }
  else if (cadena.equals("MOSTRAR_NIVEL_BEBEDERO")) // Entre las comillas se pone el texto del mensaje que se espera
  {
    digitalWrite(pinTrigger, HIGH);//Ponemos en voltaje alto(5V) el pin de "trigger"
    delayMicroseconds(10);//Esperamos en esta línea para conseguir un pulso de 10us
    digitalWrite(pinTrigger, LOW);//Ponemos en voltaje bajo(0V) el pin de "trigger"

    tiempo = pulseIn(pinEcho, HIGH);//Utilizamos la función  pulseIn() para medir el tiempo del pulso/echo
    distancia = tiempo * 0.01715; //Obtenemos la distancia considerando que la señal recorre dos veces la distancia a medir y que la velocidad del sonido es 343m/s
    if (distancia >= 140) {
      distancia = 13;
    }
    distancia = distancia * 100;
    numero2 = int(distancia);
    Serial.println("distancia ");
    Serial.println(numero2);
    delay(10);          //Nos mantenemos en esta línea durante 100ms antes de terminar el loop
  }
  else if (cadena.equals("NIVEL_BEBEDERO_CONSIGNA")) // Y así sucesivamente con todos los posibles mensajes
  {
    digitalWrite(TRIG, HIGH);
    delay(1);
    digitalWrite(TRIG, LOW);
    DURACION = pulseIn(ECO, HIGH);
    DISTANCIA = DURACION / 58.2;
    DISTANCIA = DISTANCIA * 100;
    //Serial.println(DISTANCIA);
    delay(200);
    // numero= altura- numero2;
    //Serial.println(numero2);
    if (DISTANCIA <= numero2 && 0 <= DISTANCIA)
    {
      estado = 1;
      servo1.write(90);
    }
    else {
      servo1.write(0);
    }
  }
}
